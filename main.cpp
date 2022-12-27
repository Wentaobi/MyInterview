#include <iostream>
#include <type_traits>
#include <source_location>
#include <typeinfo>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <variant>

using namespace std;

union Variant 
{
    uint8_t i8;
    uint16_t i16;
};

template<typename T, typename U>
// auto add(auto t, auto u)
auto add(T t,U u) -> decltype(t+u)
{
    return t + u;
}

class A final{
public:
    explicit A() 
    {
        cout << "Create A\n";
    }
    ~A()
    {
        cout << "Delete A\n";
    }
};

// unique pointer
template<typename T>
class UniquePointer
{
public:
    UniquePointer() = default;
    // 隐式转换
    // explicit UniquePointer(T *t):ptr_(t)
    // {}
    UniquePointer(T *t):ptr_(t)
    {}
    // delete copy constructor
    UniquePointer(const UniquePointer&) = delete;
    // delete copy
    UniquePointer& operator=(const UniquePointer&) = delete; 
    // move ctor and move copy adds noexcept keyword
    // so that compiler can optimize 
    // && ctor
    UniquePointer(UniquePointer&&rhs) noexcept
    :ptr_(rhs.ptr_)
    {
        rhs.ptr_ = nullptr;
        // exchange(rhs.ptr_, nullptr);
    }
    // move = operator
    UniquePointer& operator=(UniquePointer&& rhs) noexcept
    {
        // return move(rhs);
        swap(ptr_, rhs.ptr_);
        return *this;
    }

    T* operator->() const
    {
        return get();
    }

    T* get() const
    {
        return ptr_;
    }

    // dctor
    ~UniquePointer()
    {
        if (ptr_) 
        {
            delete ptr_;
            ptr_ = nullptr;
        }
    }

private:
    T* ptr_ = nullptr;    
};

// Any
// template<typename T>
class Any
{
public:
    template<typename T>
    Any(T t):value_(new ValueHolder<T>(t))
    {
        // value_ = t;
        // value_ = new ValueHolder<T>(t);
    }
    Any(const Any& rhs)
    {
        *this = rhs;
    }
    Any& operator=(const Any& rhs)
    {   
        value_ = rhs.value_->clone();
        return *this;
    }

    // template<typename T>
    // T* value_ptr() const
    // {
    //     if (auto p = dynamic_cast<ValueHolder<T>*>(value_))
    //     {
    //         return &(p->data);
    //     }
    //     return nullptr;
    //     throw logic_error("type error!");
    // }
    template<typename T>
    T& value() const
    {
        if (auto p = dynamic_cast<ValueHolder<T>*>(value_.get()))
        {
            return (p->data);
        }
       throw logic_error("type error!");
    }

    operator int()
    {
        return Value<int>();
    }
    
private:
    struct Value
    {
        virtual UniquePointer<Value> clone() = 0;
        virtual ~Value() {}
    };
    template<typename T>
    struct ValueHolder : Value
    {
        ValueHolder(T t):data(t) {}
        UniquePointer<Value> clone() override
        {
            return new ValueHolder<T>(data);
        }
        T data;
    };

    UniquePointer<Value> value_;
};

void print(Any a) 
{

}

int main() {
    //UniquePointer<A> a1 = new A();
    //UniquePointer<A> a2 = a1;
    //UniquePointer<A> a2 = static_cast<UniquePointer<A>&&>(a1);
    Any d = A();
    d = 1;
    try
    {
        auto v = d.value<int>();
        d = "1234";
        auto s = d.value<const char*>();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    d = "123";
    print(d);
    cout << "s: " << s << endl;

    // Variant v{0};
    // cout << "size: " << sizeof(v) << endl;
    // v.i8 = 0xff;
    // cout << "size: " << sizeof(v) << endl;
    // variant<int, double, string> u;
    // u = "1234";
    // u = 1.00;
    //cout << "variant: " << get<double>(v) << endl;
    // if (auto u1 = get_if<double>(&u)) cout << "u1: " << *u1 << endl;



    // cout << "Hello World" << endl;
    // int & -> int
    // decay_t<int&> decay_t<const int&> decay_t<volatile int&>类型退化

    // cout << is_same_v<int, double> << endl;
    // cout << is_same<int, double>::value << endl;
    // constexpr bool v = typeid(int)==typeid(int&);
    // cout << is_same_v<int, decltype(1+1)> << endl;

    // const source_location& location = source_location::current();
    // cout << location.line() << endl;
    // cout << location.file_name() << endl;;
    // cout << location.function_name() << endl;
    return 0;
}