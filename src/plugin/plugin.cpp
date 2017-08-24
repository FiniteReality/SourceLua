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
#include <lua/libraries/console.hpp>

namespace SourceLua
{

const char* Plugin::RunLuaString(const char* code)
{
    Lua::Script script {_G, "console"};

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


int luaopen_plugin(lua_State* _G)
{
    luaopen_base(_G);
    luaopen_bit(_G);
    luaopen_math(_G);
    // TODO: should we disable this? it may allow malicious
    // files to be loaded
    luaopen_package(_G);
    luaopen_string(_G);
    luaopen_table(_G);

    Lua::Libraries::luaopen_thread(_G);
    Lua::Libraries::luaopen_console(_G);

    return 0;
}

void Plugin::LoadLua()
{
    lua_cpcall(_G, luaopen_plugin, nullptr);

    lua_createtable(_G, 0, 1);
    lua_setfield(_G, LUA_REGISTRYINDEX, SOURCELUA_SCRIPT_KEY);

    lua_createtable(_G, 0, 1);
    lua_setfield(_G, LUA_REGISTRYINDEX, SOURCELUA_SCHEDULER_CACHE_KEY);

    lua_pushlightuserdata(_G, _scheduler.get());
    lua_setfield(_G, LUA_REGISTRYINDEX, SOURCELUA_SCHEDULER_KEY);
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
