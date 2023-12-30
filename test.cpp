// compile: g++ test.cpp && ./a.out

#include "reflect.hpp"
#include <iostream>

using namespace reflection;

struct MyStruct
{
REFLECT(
    (int) x,
    (bool) y
)};

static_assert(
    sizeof_fields<MyStruct>::value == 5,
"");

static_assert(
    std::is_same<
        reflector::field_t<0, MyStruct>::type, // int x;
        int>
    ::value == true,
"");

//static_assert(has_x<MyStruct>::value == true, "");

int main()
{   
    MyStruct x{ 10, true };
    
    std::cout << to_string(x) << '\n';

    return 0;
}