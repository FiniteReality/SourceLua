#ifndef _objcommon_hpp_
#define _objcommon_hpp_

#include <common/logging.hpp>
#include <common/luajit.hpp>

namespace SourceLua
{
namespace Lua
{
namespace Objects
{

template<typename T>
struct ClassDefinitionMethods {
    static void RegisterMetamethods(lua_State* L)
    {
        LogMessage<LogLevel::Debug>("No metamethods to register");
    };
    static void RegisterMethods(lua_State* L)
    {
        LogMessage<LogLevel::Debug>("No methods to register");
    }
    static void RegisterSubtypes(lua_State* L)
    {
        LogMessage<LogLevel::Debug>("No subtypes to register");
    }
};

template<typename T>
struct ClassDefinition
{
    private:
        using methods = ClassDefinitionMethods<T>;

        struct wrapper
        {
            T* value;
        };

        static const char* const type_name;

        static inline wrapper* CreateWrapper(lua_State* L)
        {
            wrapper* value = static_cast<wrapper*>(
                lua_newuserdata(L, sizeof(wrapper)));

            luaL_getmetatable(L, type_name);
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
                luaL_checkudata(L, idx, type_name));

            return value->value;
        }

        static void RegisterType(lua_State* L)
        {
            LogMessage<LogLevel::Debug>("Registering type %s", type_name);
            luaL_newmetatable(L, type_name);

            LogMessage<LogLevel::Debug>(
                "Registering metamethods for %s", type_name);
            methods::RegisterMetamethods(L);

            lua_pushliteral(L, "This metatable is locked");
            lua_setfield(L, -2, "__metatable");

            lua_newtable(L);
            LogMessage<LogLevel::Debug>(
                "Registering methods for %s", type_name);
            methods::RegisterMethods(L);
            lua_setfield(L, -2, "__index");

            lua_pop(L, 1); // pop the metatable

            LogMessage<LogLevel::Debug>(
                "Registering subtypes for %s", type_name);
            methods::RegisterSubtypes(L);
        }
};

#define DEFINE_CLASS_METHODS(type, ...) \
    namespace SourceLua { \
    namespace Lua { \
    namespace Objects { \
    template <> \
    void ClassDefinitionMethods<type>::RegisterMethods(lua_State* L) \
    { __VA_ARGS__ } \
    } } }

#define DEFINE_CLASS_METAMETHODS(type, ...) \
    namespace SourceLua { \
    namespace Lua { \
    namespace Objects { \
    template <> \
    void ClassDefinitionMethods<type>::RegisterMetamethods(lua_State* L) \
    { __VA_ARGS__ } \
    } } }

#define DEFINE_CLASS_SUBTYPES(type, ...) \
    namespace SourceLua { \
    namespace Lua { \
    namespace Objects { \
    template <> \
    void ClassDefinitionMethods<type>::RegisterSubtypes(lua_State* L) \
    { __VA_ARGS__ } \
    } } }

#define DEFINE_CLASS(type) \
    namespace SourceLua { \
    namespace Lua { \
    namespace Objects { \
    template <> \
    const char* const ClassDefinition<type>::type_name = #type; \
    } } }

#define DEFINE_CLASS_NAME(type, name) \
    namespace SourceLua { \
    namespace Lua { \
    namespace Objects { \
    template <> \
    const char* const ClassDefinition<type>::type_name = name; \
    } } }

}
}
}

#endif /* _objcommon_hpp_ */
