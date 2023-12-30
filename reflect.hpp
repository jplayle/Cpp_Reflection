#pragma once

// to-do
// - has_member structs

#include <string>

/* MACROS */

#ifndef REFLECT_LIB_ACTIVE
#define REFLECT_LIB_ACTIVE
#endif

/* generic macros */

#define XPASTE(a,b) PASTE(a,b)
#define PASTE(a,b)  a ## b
#define EAT(...)
#define REM(...)    __VA_ARGS__
#define STRIP(x)    EAT x
#define PAIR(x)     REM x
#define XSTR(s)     STR(s)
#define STR(s)      #s

/* variadic expansion macros */

#define PP_NARG(...)  PP_NARG_(__VA_ARGS__, PP_RSEQ_N())
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)

#define PP_ARG_N(                                \
        _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,  \
        _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
        _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
        _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
        _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
        _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
        _61,_62,_63,N,...) N

#define PP_RSEQ_N()                    \
        63,62,61,60,                   \
        59,58,57,56,55,54,53,52,51,50, \
        49,48,47,46,45,44,43,42,41,40, \
        39,38,37,36,35,34,33,32,31,30, \
        29,28,27,26,25,24,23,22,21,20, \
        19,18,17,16,15,14,13,12,11,10, \
        9,8,7,6,5,4,3,2,1,0

#define APPLYX1(a) X(0,a)
#define APPLYX2(a,b) X(0,a) X(1,b)
#define APPLYX3(a,b,c) X(0,a) X(1,b) X(2,c)

#define APPLYX_(M, ...) M(__VA_ARGS__)

/*
* struct to hold info about a member variable
*/
#define X(N, arg)                         \
PAIR(arg);                                \
template<class C>                         \
struct field_data<N, C>                   \
{                                         \
    using type = decltype(C::STRIP(arg)); \
    constexpr static bool SET = true;     \
    constexpr static std::size_t size =   \
        sizeof(C::STRIP(arg));            \
                                          \
    C& self;                              \
                                          \
    field_data(C& c) :                    \
    self(c) {}                            \
                                          \
    auto& get() const                     \
    {                                     \
        return self.STRIP(arg);           \
    }                                     \
                                          \
    constexpr const char* name() const    \
    {                                     \
        return XSTR(STRIP(arg));          \
    }                                     \
};                                        \
HAS_MEM_VAR_STRUCTS(arg)

#define HAS_MEM_VAR_STRUCTS(arg)          \
template<class C, typename T = int>       \
struct XPASTE(has_member_, STRIP(arg))    \
{                                         \
    constexpr static bool value = false;  \
};                                        \
                                          \
template<class C>                         \
struct XPASTE(has_member_, STRIP(arg))    \
<C, decltype((void)C::STRIP(arg), 0)>     \
{                                         \
    constexpr static bool value = true;   \
};

/*
* wrap member variables in this macro to enable reflection.
*/
#define REFLECT(...)                      \
constexpr static int N_FIELDS =           \
    PP_NARG(__VA_ARGS__);                 \
                                          \
friend struct reflector;                  \
                                          \
template<int N, class C>                  \
struct field_data                         \
{                                         \
    constexpr static bool SET = false;    \
};                                        \
                                          \
APPLYX_(XPASTE(APPLYX, PP_NARG(__VA_ARGS__)), __VA_ARGS__)

/* --- */

namespace reflection
{
    /* UTILS */

    /*
    * enables access to private and protected member variables
    */
    struct reflector
    {
        template<int N, class C> using field_t = typename C::template field_data<N, C>;

        template<int N, class C>
        static field_t<N, C> get_field_data(C& c)
        {
            return typename C::template field_data<N, C>(c);
        }
    };

    template<int N, class C>
    struct sizeof_fields_helper
    {
        constexpr static std::size_t value = reflector::field_t<N, C>::size + sizeof_fields_helper<N-1, C>::value;
    };

    template<class C>
    struct sizeof_fields_helper<0, C>
    {
        constexpr static std::size_t value = reflector::field_t<0, C>::size;
    };
    
    template<class C, int N, bool S>
    struct for_each_helper
    {
        template<typename F>
        constexpr static void apply_visitor(C& c, F&& f)
        {
            f(reflector::get_field_data<N>(c));
            
            for_each_helper<C, N+1, (N+1 < C::N_FIELDS)>::apply_visitor(c, f);
        }
    };
    
    template<class C, int N>
    struct for_each_helper<C, N, false>
    {
        template<typename F>
        constexpr static void apply_visitor(C& c, F&& f)
        {
        }
    };

    /* PREMADE TOOLS */

    /*
    * recursively sum sizeof() for all member variables
    */
    template<class C>
    struct sizeof_fields : sizeof_fields_helper<C::N_FIELDS-1, C>
    {};
    
    /*
    * loop all member variables (optional: apply a visitor function)
    */
    template<class C>
    struct for_each : for_each_helper<C, 0, (C::N_FIELDS > 0)>
    {};
    
    /*
    * string of delimited field=value pairs for each member variable
    */
    template<class C>
    std::string to_string(C& c, const char delim = '|')
    {
        std::string s;
        
        for_each<C>::apply_visitor(c, [&](auto f)
        {
            s += std::string(f.name()) +
                "=" +
                std::to_string(f.get()) +
                delim;
        });
        
        return s.substr(0, s.length() - 1);
    }
}