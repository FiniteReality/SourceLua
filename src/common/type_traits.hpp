#ifndef _type_traits_hpp_
#define _type_traits_hpp_

namespace SourceLua
{
template <typename T>
struct class_name {
    static const char* const name;
};

#define DEFINE_CLASS_NAME(X) \
    template <> struct class_name<X> { }; \
    const char* const class_name<X>::name = #X
}

#endif /* _type_traits_hpp_ */
