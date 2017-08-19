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
Lua::Script* Plugin::GetScriptFromState(lua_State* L)
{
    lua_getfield(_G, LUA_REGISTRYINDEX, SOURCELUA_SCRIPT_KEY);

    if (lua_pushthread(L))
        throw new std::runtime_error(
            "GetScriptFromState called on main state");

    // Move thread to global state so we can call gettable
    lua_xmove(L, _G, 1);
    lua_gettable(_G, -2);

    if (!lua_islightuserdata(_G, -1))
        throw new std::runtime_error(
            "GetScriptFromState script was not light userdata");

    auto script = static_cast<Lua::Script*>(lua_touserdata(_G, -1));

    // Pop table and userdata
    lua_pop(_G, 2);

    return script;
}

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

    lua_pushlightuserdata(_G, _scheduler.get());
    lua_setfield(_G, LUA_REGISTRYINDEX, SOURCELUA_SCHEDULER_KEY);
}
}