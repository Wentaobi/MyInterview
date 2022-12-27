#include <iostream>
using namespace std;
template <class T>
class unique_ptr
{
public: 
    //模板成员指针参数赋值
    unique_ptr(T* ptr=nullptr) : obj_ptr(ptr) {}
    //不传右值的带类参数构造函数需要disable
    unique_ptr(const unique_ptr& other) = delete;
    //不传右值的带参数操作符重载函数需要disable
    unique_ptr& operator = (const unique_ptr& other) = delete;   
    // move constructor
    unique_ptr(unique_ptr&& other) noexcept : obj_ptr(other.obj_ptr) {other.obj_ptr = nullptr;}
    // move assignment operator
    unique_ptr& operator = (unique_ptr&& other)
    {
        if (this != &other) 
        {
            delete obj_ptr;
            this->obj_ptr = other.obj_ptr;
            other.obj_ptr = nullptr;
        }
        return *this;
    }
    // operator overloading
    // 指向成员变量，模板指针类型
    T* operator -> () const {return obj_ptr;}
    // 类解引用，对成员变量指针解引用
    T& operator * () const {return *obj_ptr;}
    T* get() const {return obj_ptr;}
    operator bool () const {return obj_ptr != nullptr;}
    void reset(T* ptr=nullptr) 
    {
        if (ptr != obj_ptr)
        {
            delete obj_ptr;
            obj_ptr = ptr;
        }
    }
    ~unique_ptr() {delete obj_ptr;}
private:
    T* obj_ptr;
};


int main()
{
    unique_ptr<int> p(new int(100));
    cout << "get(): " << p.get() << endl;
    cout << "p: " << *p << endl;
    cout << "reset: " << endl;
    p.reset(new int(20));
    cout << "p: " << *p << endl;
    cout << "get(): " << p.get() << endl;

}

// wrong!
// unique_ptr(unique_ptr& other) : unique_ptr(other.obj_ptr) 
// {
//     other.obj_ptr = nullptr;
// } 