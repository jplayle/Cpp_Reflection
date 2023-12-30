C++ Reflection Tools

    #include "reflect.hpp"
    
    struct  A
    {
    REFLECT(
	    (int) x,
	    (bool) y
	)};

Loop fields and apply a visitor function:

    reflection::for_each<A>::apply_visitor(a, [&](auto f)
    {
	    f.name;  // member name e.g. "x"
	    f.size;  // sizeof(f)
	    f.get(); // ref to value stored in the field
    });
 
to_string() function:

    A a{ 10, true };
    reflection::to_string(a); // x=10|y=1

Checker whether members are defined (constexpr):

    reflection::member_view<A>::HAS(x);   // true
    reflection::member_view<A>::has("x"); // true
    reflection::member_view<A>::has("q"); // false

Get size of all fields (constexpr):

    sizeof_fields<A>::value; // sizeof(int) + sizeof(bool)

Query member types (by order of declaration, zero indexed):

    std::is_same<int, reflection::reflector::field_t<0, A>::type>::value; // true
