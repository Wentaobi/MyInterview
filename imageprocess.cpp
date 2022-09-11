#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
// Mock image class
class Image
{
public:
      Image(const std::string &name) : name_(name) {}
      const std::string &name() const { return name_; }

private:
      const std::string name_;
};
// Sample image manipulation library.
namespace ImgLib
{
    void convertToGrayScale(Image &img)
    {
          std::cout << img.name() << ": Converting to gray scale" << std::endl;
    }
    void blur(Image &img, float factor)
    {
          std::cout << img.name() << ": Blurring with factor " << factor << std::endl;
    }
    void resize(Image &img, int x, int y)
    {
          std::cout << img.name() << ": Resizing img with x = " << x << " and y = " << y << std::endl;
    }
    void blend(Image &img, const Image &other)
    {
          std::cout << img.name() << ": Blending with " << other.name() << std::endl;
    }
    /* Potentially many more! */
} // namespace ImgLib
////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Pipeline 组件抽象 ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class ImageTransformationHandle
{
public:
      virtual void transform(Image &image) const = 0;
};
class ImageTransformationFunction
{
public:
      virtual std::string getName() const = 0;
      virtual ImageTransformationHandle *generateHandle(std::istringstream &params) const = 0;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// Pipeline ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class ImageTransformationPipeline
{
public:
      ImageTransformationPipeline(const std::vector<std::string> &cmds)
    {
            for (const std::string &cmd : cmds)
        {
                  std::istringstream iss(cmd);
                  std::string fname;
                  iss >> fname;
                  ImageTransformationFunction *func = FindFunction(fname);
                  if (func == nullptr)
            {
                        throw std::runtime_error("Invalid image transformation");
                      
            }
                  pipeline_.emplace_back(func->generateHandle(iss));
               
        }
          
    }
      void transform(Image &image)
    {
            for (const auto &handle : pipeline_)
        {
                  handle->transform(image);
               
        }
          
    }
      void transform(std::vector<Image> &images)
    {
            for (Image &image : images)
        {
                  transform(image);
               
        }
          
    }

private:
      std::vector<std::unique_ptr<ImageTransformationHandle>> pipeline_;
       // 为了简便把function resolving写在这里，其实最好再写一个class ImageTransformationFactory
  static std::unordered_map<
      std::string, std::unique_ptr<ImageTransformationFunction>> functions_;
      static ImageTransformationFunction *FindFunction(const std::string &name)
    {
            auto it = functions_.find(ToLowerCase(name));
            if (it == functions_.end())
        {
                  return nullptr;
               
        }
        else
        {
                  return it->second.get();
               
        }
          
    }
      static std::string ToLowerCase(const std::string &str)
    {
            std::string ret(str.size(), ' ');
            std::transform(str.begin(), str.end(), ret.begin(), tolower);
            return ret;
          
    }

public:
      template <typename Function>
  static void RegisterFunction()
    {
            Function *function = new Function();
            functions_.emplace(ToLowerCase(function->getName()), function);
          
    }
};
std::unordered_map<std::string, std::unique_ptr<ImageTransformationFunction>>
    ImageTransformationPipeline::functions_;
////////////////////////////////////////////////////////////////////////////////
/////////////////////////// Pipeline 各个组件的实现 //////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Convert to gray scale.
class ConvertToGrayScaleHandle : public ImageTransformationHandle
{
public:
      void transform(Image &image) const override
    {
            ImgLib::convertToGrayScale(image);
          
    }
};
class ConvertToGrayScaleFunction : public ImageTransformationFunction
{
public:
      std::string getName() const override
    {
            return "ConvertToGrayScale";
          
    }
      ImageTransformationHandle *generateHandle(std::istringstream &params) const override
    {
            return new ConvertToGrayScaleHandle();
          
    }
};
// Blur.
class BlurHandle : public ImageTransformationHandle
{
public:
      BlurHandle(const float factor) : factor_(factor) {}
      void transform(Image &image) const override
    {
            ImgLib::blur(image, factor_);
          
    }

private:
      const float factor_;
};
class BlurFunction : public ImageTransformationFunction
{
public:
      std::string getName() const override
    {
            return "Blur";
          
    }
      ImageTransformationHandle *generateHandle(std::istringstream &params) const override
    {
            float factor;
            params >> factor;
            return new BlurHandle(factor);
          
    }
};
// Resize.
class ResizeHandle : public ImageTransformationHandle
{
public:
      ResizeHandle(const int x, const int y) : x_(x), y_(y) {}
      void transform(Image &image) const override
    {
            ImgLib::resize(image, x_, y_);
          
    }

private:
      const int x_;
      const int y_;
};
class ResizeFunction : public ImageTransformationFunction
{
public:
      std::string getName() const override
    {
            return "Resize";
          
    }
      ImageTransformationHandle *generateHandle(std::istringstream &params) const override
    {
            int x, y;
            params >> x >> y;
            return new ResizeHandle(x, y);
          
    }
};
// Blend.
class BlendWithHandle : public ImageTransformationHandle
{
public:
      BlendWithHandle(const std::string &url) : other_(url) {}
      void transform(Image &image) const override
    {
            ImgLib::blend(image, other_);
          
    }

private:
      const Image other_;
};
class BlendWithFunction : public ImageTransformationFunction
{
public:
      std::string getName() const override
    {
            return "BlendWith";
          
    }
      ImageTransformationHandle *generateHandle(std::istringstream &params) const override
    {
            std::string url;
            params >> url;
            return new BlendWithHandle(url);
          
    }
};
int main()
{
       // Read the config file
  std::vector<std::string> cmds = {
      "ConvertToGrayScale",
      "Blur 3.",
      "Resize 256 256",
      "BlendWith http://www.solstation.com/stars/earth3au.jpg"
  };
      std::vector<Image> imgs = {
      Image("one"), Image("two"), Image("three")
  };
       // Your work goes here.
   // Register transformations.
  ImageTransformationPipeline::RegisterFunction<ConvertToGrayScaleFunction>();
      ImageTransformationPipeline::RegisterFunction<BlurFunction>();
      ImageTransformationPipeline::RegisterFunction<ResizeFunction>();
      ImageTransformationPipeline::RegisterFunction<BlendWithFunction>();
       // Construct a pipeline instance from the commands.
  ImageTransformationPipeline pipeline(cmds);
       // Apply the transformations to imgs.
  pipeline.transform(imgs);
      return 0;
}
