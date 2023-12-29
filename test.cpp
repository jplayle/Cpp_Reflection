// compile: g++ test.cpp && ./a.out

#include "reflect.hpp"
#include <iostream>

struct MyStruct
{
REFLECT(
    (int) x,
    (bool) y
)};

//static_assert(reflection::sizeof_fields<MyStruct>() == 5, "");

int main()
{   
    MyStruct x{ 10, true };

    int a[reflection::sizeof_fields<MyStruct>()] = {};
    
    std::cout << reflection::to_string(x) << '\n';
    std::cout << reflection::sizeof_fields<MyStruct>() << '\n';

    return 0;
}