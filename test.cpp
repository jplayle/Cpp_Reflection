// compile: g++ test.cpp && ./a.out

#include "reflect.hpp"
#include <iostream>

using namespace reflection;

struct A
{
REFLECT(
    (int) x,
    (bool) y
)};

// query sum size of fields
static_assert(sizeof_fields<A>::value == 5,     "");

// query existence of members in struct by macro or stringified member name
static_assert(has_member<A>::HAS(x)   == true,  "");
static_assert(has_member<A>::has("x") == true,  "");

static_assert(has_member<A>::HAS(f)   == false, "");
static_assert(has_member<A>::has("i") == false, "");

// observe type of member at position
static_assert( std::is_same<int, reflector::field_t<0, A>::type>::value == true, "");

int main()
{   
    A a{ 10, true };
    
    std::cout << to_string(a) << '\n'; // hello=10|world=1

    return 0;
}