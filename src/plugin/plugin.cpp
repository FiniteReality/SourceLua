/*
 * Main plugin code
 */

#include <stdexcept>

#include <common/luajit.hpp>
#include <common/logging.hpp>
#include <common/version.hpp>
#include <lua/lua.hpp>
#include <plugin/plugin.hpp>

#include <lua/libraries/thread.hpp>

namespace SourceLua
{

const char* Plugin::RunLuaString(const char* code)
{
    Lua::Script script{_G, "console"};

    script.Run(code);

    return nullptr;
}

#ifdef SOURCELUA_DEBUG
void Plugin::CausePanic()
{
    lua_pushnil(_G);
    lua_call(_G, 0, 0);
}
#endif /* SOURCELUA_DEBUG */

void Plugin::LoadLua()
{
    // TODO: replace this with a more secure approach (loading all libs means
    // we can potentially do bad things like turning JIT off, loading unknown
    // libraries, etc.)
    luaL_openlibs(_G);
    Lua::Threading::luaopen_thread(_G);

    lua_createtable(_G, 0, 1);
    lua_setfield(_G, LUA_REGISTRYINDEX, SOURCELUA_SCRIPT_KEY);

    lua_createtable(_G, 0, 1);
    lua_setfield(_G, LUA_REGISTRYINDEX, SOURCELUA_SCHEDULER_CACHE_KEY);

    lua_pushlightuserdata(_G, _scheduler.get());
    lua_setfield(_G, LUA_REGISTRYINDEX, SOURCELUA_SCHEDULER_KEY);
}

}
