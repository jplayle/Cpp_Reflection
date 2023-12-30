C++ Reflection Tools

    #include "reflect.hpp"
    
    struct  A
    {
    REFLECT(
	    (int) x,
	    (bool) y
	)};

Query members (constexpr):

    reflection::member_view<A>::HAS(x);   // true
    reflection::member_view<A>::has("x"); // true
    reflection::member_view<A>::has("q"); // false

Get size of all fields (constexpr):

    sizeof_fields<A>::value; // sizeof(int) + sizeof(bool)

