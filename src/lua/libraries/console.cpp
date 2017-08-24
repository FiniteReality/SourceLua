#include <common/luajit.hpp>
#include <common/source.hpp>

#include <lua/libraries/console.hpp>

namespace SourceLua
{
namespace Lua
{
namespace Libraries
{

// TODO:
// - Support messages to client

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

static const luaL_Reg console_funcs[] =
{
    {"message", LogMessage},
    {"devMessage", LogDevMessage},
    {"warning", LogWarning},
    {"devWarning", LogDevWarning},
    // TODO: support writing to client consoles
    {NULL, NULL}
};

int luaopen_console(lua_State* L)
{
    luaL_register(L, "console", console_funcs);
    return 1;
}

}
}
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
