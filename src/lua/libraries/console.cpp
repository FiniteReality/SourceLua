#include <source/tier0/dbg.hpp>

#include <common/luajit.hpp>

#include <lua/libraries/console.hpp>

namespace SourceLua
{
namespace Lua
{
namespace Libraries
{

int LogMessage(lua_State* L)
{
    const char* msg = luaL_checkstring(L, 1);

    Msg("%s\n", msg);
    return 0;
}

int LogDevMessage(lua_State* L)
{
    const char* msg = luaL_checkstring(L, 1);

    DevMsg("%s\n", msg);
    return 0;
}

int LogWarning(lua_State* L)
{
    const char* msg = luaL_checkstring(L, 1);

    Warning("%s\n", msg);
    return 0;
}

int LogDevWarning(lua_State* L)
{
    const char* msg = luaL_checkstring(L, 1);

    DevWarning("%s\n", msg);
    return 0;
}

static int PrintToLog(lua_State* L)
{
    int nargs = lua_gettop(L);

    luaL_Buffer buffer;
    luaL_buffinit(L, &buffer);

    for (int i = 1; i <= nargs; i++)
    {
        if (!luaL_callmeta(L, i, "__tostring"))
        {
            switch(lua_type(L, i))
            {
                case LUA_TNUMBER:
                    lua_pushstring(L, lua_tostring(L, i));
                    break;
                case LUA_TSTRING:
                    lua_pushvalue(L, i);
                    break;
                case LUA_TBOOLEAN:
                    lua_pushstring(L,
                        lua_toboolean(L, i) == 0 ? "false" : "true");
                    break;
                case LUA_TNIL:
                    lua_pushliteral(L, "nil");
                    break;
                default:
                    lua_pushfstring(L, "%s: %p",
                        lua_typename(L, lua_type(L, i)),
                        lua_topointer(L, i));
                    break;
            }
        }

        luaL_addvalue(&buffer);
        if (i < nargs)
            luaL_addstring(&buffer, "\t");
    }

    luaL_pushresult(&buffer);
    const char* message = lua_tostring(L, -1);

    Msg("%s\n", message);

    return 0;
}

static const luaL_Reg console_funcs[] =
{
    {"message", LogMessage},
    {"devMessage", LogDevMessage},
    {"warning", LogWarning},
    {"devWarning", LogDevWarning},
    {nullptr, nullptr}
};

int luaopen_console(lua_State* L)
{
    luaL_register(L, "console", console_funcs);
    // Overwrite 'print' to log messages using LogMessage
    lua_register(L, "print", PrintToLog);
    return 1;
}

}
}
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
