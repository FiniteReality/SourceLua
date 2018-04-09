#ifndef _objcommon_hpp_
#define _objcommon_hpp_

#include <common/luajit.hpp>
#include <common/type_traits.hpp>

namespace SourceLua
{
namespace Lua
{
namespace Objects
{

template<typename T>
struct ClassDefinitionMethods {
    static void RegisterMetamethods(lua_State* L);
    static void RegisterMethods(lua_State* L);
};

template<typename T>
struct ClassDefinition
{
    private:
        using ClassType = T;
        using type_name = class_name<T>;
        using methods = ClassDefinitionMethods<T>;

        struct wrapper
        {
            T* value;
        };

        static inline wrapper* CreateWrapper(lua_State* L)
        {
            wrapper* value = static_cast<wrapper*>(
                lua_newuserdata(L, sizeof(wrapper)));

            luaL_getmetatable(L, type_name::name);
            lua_setmetatable(L, -2);

            return value;
        }

    public:
        static void PushValue(lua_State* L, T* value)
        {
            wrapper* wrapper = CreateWrapper(L);
            wrapper->value = value;
        }

        static T* CheckValue(lua_State* L, int idx)
        {
            wrapper* value = static_cast<wrapper*>(
                luaL_checkudata(L, idx, type_name::name));

            return value->value;
        }

        static void RegisterType(lua_State* L)
        {
            luaL_newmetatable(L, type_name::name);

            methods::RegisterMetamethods(L);

            lua_pushliteral(L, "This metatable is locked");
            lua_setfield(L, -2, "__metatable");

            lua_newtable(L);
            methods::RegisterMethods(L);
            lua_setfield(L, -2, "__index");
        }
};

#define DEFINE_CLASS(type, ...) \
    namespace SourceLua { \
    template <> \
    const char* const class_name<type>::name = #type; \
    namespace Lua { namespace Objects { \
    template <> struct ClassDefinitionMethods<type> \
    { __VA_ARGS__ }; \
    } } }

}
}
}

#endif /* _objcommon_hpp_ */
