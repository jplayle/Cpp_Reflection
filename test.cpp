// compile: g++ test.cpp && ./a.out

#include "reflect.hpp"
#include <iostream>

struct A
{
REFLECT(
    (int) x,
    (bool) y
)};

// query sum size of fields
static_assert(reflection::sizeof_fields<A>::value == sizeof(int) + sizeof(bool), "");

// query existence of members in struct by macro or stringified member name
static_assert(reflection::member_view<A>::HAS(x)   == true,  "");
static_assert(reflection::member_view<A>::has("x") == true,  "");

static_assert(reflection::member_view<A>::HAS(f)   == false, "");
static_assert(reflection::member_view<A>::has("i") == false, "");

// observe type of member at position
static_assert( std::is_same<int, reflection::reflector::field_t<0, A>::type>::value == true, "");

int main()
{   
    A a{ 10, true };
    
    std::cout << reflection::to_string(a) << '\n'; // x=10|y=1

    return 0;
}