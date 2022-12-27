#include <iostream>
using namespace std;

/*
通过将函数模板的形参类型设置为 T&&，我们可以很好地解决接收左、右值的问题。但除此之外，
还需要解决一个问题，即无论传入的形参是左值还是右值，对于函数模板内部来说，形参既有名称又能寻址，
因此它都是左值。那么如何才能将函数模板接收到的形参连同其左、右值属性，一起传递给被调用的函数呢？
*/

/*
完美转发： 函数模板可以将自己的参数”完美“地转发给内部调用的其他函数。
所谓完美，即不仅能准确地转发参数的值，还能保证被转发参数的左、右值属性不变。
*/

/*
万能引用：C++11标准中，通常情况下，右值引用形式的参数只能接收右值，不能接收左值，
但对于函数模板中使用右值引用语法定义的参数来说，它不再遵守这一规定，
既可以接收右值，也可以接收左值。此时的右值引用就是万能引用。
*/

/*
引用折叠规则：
当实参为左值或者左值引用(int&)时，函数模板中T&&将转变为int&,(int& && = int&)
当实参为右值或者右值引用时(int&&),函数模板中T&&将转变为int&&, (int&& && = int&&)
在实现完美转发时，只要函数模板的类型为T&&，则C++可以自行准确判定出实际传入的实参是左值还是右值。 
*/

void subfunc(int& t)
{
    cout << "lvalue: " << t << endl;
}

void subfunc(int&& t)
{
    cout << "rvalue: " << t << endl;
}

template<class T>
void func(T&& t) 
{
    subfunc(std::forward<T>(t));
}

/*
std::remove_reference<T>::type 含义就是获得去掉引用的参数类型
然后对传入的参数进行强制类型转换，转换的目标类型符合引用折叠规则
*/
#if 0
template <typename T>
T&& forward(typename std::remove_reference<T>::type& param)
{
    return static_cast<T&&>(param);
}

template <typename T>
T&& forward(typename std::remove_reference<T>::type&& param)
{
    return static_cast<T&&>(param);
}
#endif

int main()
{
    int i = 5;
    func(5);
    func(i);
    return 0;
}