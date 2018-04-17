#include <source/igameevents.hpp>

#include "common.hpp"

/*
 * HACK: This exists to wrap IGameEvent temporarily. It should be removed in
 * favour of automatically deserializing event members into tables.
 */

using namespace SourceLua::Lua;

using LuaGameEvent = Objects::ClassDefinition<IGameEvent>;

int GameEventEqual(lua_State* L)
{
    IGameEvent* lhs = LuaGameEvent::CheckValue(L, 1);
    IGameEvent* rhs = LuaGameEvent::CheckValue(L, 2);

    lua_pushboolean(L, lhs == rhs ? 1 : 0);
    return 1;
}

int GameEventTostring(lua_State* L)
{
    IGameEvent* event = LuaGameEvent::CheckValue(L, 1);

    // TODO: better tostring() result for this
    lua_pushstring(L, event->GetName());
    return 1;
}

#define CheckKey(L) \
    IGameEvent* event = LuaGameEvent::CheckValue(L, 1); \
    const char* key = luaL_checkstring(L, 2); \
    \
    if (event->IsEmpty(key)) \
        return luaL_error(L, "Key %s does not exist", key)

int GameEventGetBool(lua_State* L)
{
    CheckKey(L);

    lua_pushboolean(L, event->GetBool(key) ? 1 : 0);
    return 1;
}


// TODO: figure out a way to merge these two

int GameEventGetInt(lua_State* L)
{
    CheckKey(L);

    lua_pushnumber(L, event->GetInt(key));
    return 1;
}

int GameEventGetFloat(lua_State* L)
{
    CheckKey(L);

    lua_pushnumber(L, event->GetFloat(key));
    return 1;
}

int GameEventGetString(lua_State* L)
{
    CheckKey(L);

    lua_pushstring(L, event->GetString(key));
    return 1;
}

#undef CheckKey

DEFINE_CLASS_METAMETHODS(IGameEvent,
{
    static luaL_Reg gameevent_metamethods[] =
    {
        {"__eq", GameEventEqual},
        {"__tostring", GameEventTostring},
        {nullptr, nullptr}
    };

    luaL_register(L, nullptr, gameevent_metamethods);
})

DEFINE_CLASS_METHODS(IGameEvent,
{
    static luaL_Reg gameevent_methods[] =
    {
        {"GetBool", GameEventGetBool},
        {"GetInt", GameEventGetInt},
        {"GetFloat", GameEventGetFloat},
        {"GetString", GameEventGetString},
        {nullptr, nullptr}
    };

    luaL_register(L, nullptr, gameevent_methods);
})

DEFINE_CLASS(IGameEvent)
