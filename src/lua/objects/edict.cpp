#include <lua/objects/common.hpp>

#include <edict.h>

using namespace SourceLua::Lua;

using LuaEdict = Objects::ClassDefinition<edict_t>;

int EdictEqual(lua_State* L)
{
    edict_t* lhs = LuaEdict::CheckValue(L, 1);
    edict_t* rhs = LuaEdict::CheckValue(L, 2);

    // TODO: is this correct? should we check entity index too?
    lua_pushboolean(L, lhs == rhs ? 1 : 0);
    return 1;
}

int EdictTostring(lua_State* L)
{
    edict_t* edict = LuaEdict::CheckValue(L, 1);

    lua_pushstring(L, edict->GetClassName());
    return 1;
}

int EdictIsFree(lua_State* L)
{
    edict_t* edict = LuaEdict::CheckValue(L, 1);

    lua_pushboolean(L, edict->IsFree() ? 1 : 0);
    return 1;
}
int EdictGetFreeTime(lua_State* L)
{
    edict_t* edict = LuaEdict::CheckValue(L, 1);

    lua_pushnumber(L, edict->freetime);
    return 1;
}

int EdictGetClassName(lua_State* L)
{
    edict_t* edict = LuaEdict::CheckValue(L, 1);

    lua_pushstring(L, edict->GetClassName());
    return 1;
}

DEFINE_CLASS(
    edict_t,
    static void RegisterMetamethods(lua_State* L)
    {
        static luaL_Reg[] metamethods{
            {"__eq", EdictEqual},
            {"__tostring", EdictTostring},
            {nullptr, nullptr}
        };

        luaL_register(L, nullptr, metamethods);
    };
    static void RegisterMethods(lua_State* L)
    {
        static luaL_Reg methods[]{
            {"isFree", EdictIsFree},

            {"getFreeTime", EdictGetFreeTime},
            {"getClassName", EdictGetClassName},

            {nullptr, nullptr}
        };

        luaL_register(L, nullptr, methods);
    };
)

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
