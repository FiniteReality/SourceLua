#include <stdexcept>

#include <common/luajit.hpp>
#include <common/logging.hpp>
#include <common/version.hpp>

#include <lua/script.hpp>

#include <lua/objects/common.hpp>

#include <lua/libraries/console.hpp>
#include <lua/libraries/event.hpp>
#include <lua/libraries/thread.hpp>

#include <plugin/plugin.hpp>

namespace SourceLua
{

Plugin::Plugin()
    : _G{luaL_newstate(), [](lua_State* L){ lua_close(L); }},
    _gameTickEvent{Lua::Libraries::GetEvent("game_ticked")},
    _levelChangingEvent{Lua::Libraries::GetEvent("level_changing")},
    _levelChangedEvent{Lua::Libraries::GetEvent("level_changed")},
    _pauseEvent{Lua::Libraries::GetEvent("paused")},
    _unPauseEvent{Lua::Libraries::GetEvent("unpaused")}
{
}

IGameEventManager2* Plugin::GetEventManager()
{
    return _eventManager;
}

void Plugin::RunLuaString(const char* code)
{
    Lua::Script console{_G.get(), "console"};

    console.Run(code);
}

#ifdef SOURCELUA_DEBUG
void Plugin::CausePanic()
{
    lua_pushnil(_G.get());
    lua_call(_G.get(), 0, 0);
}
#endif /* SOURCELUA_DEBUG */


int luaopen_plugin(lua_State* _G)
{
    luaL_openlibs(_G);

    Lua::Objects::ClassDefinition<edict_t>::RegisterType(_G);
    Lua::Objects::ClassDefinition<Lua::Event>::RegisterType(_G);
    Lua::Objects::ClassDefinition<IGameEvent>::RegisterType(_G);

    Lua::Libraries::luaopen_console(_G);
    Lua::Libraries::luaopen_event(_G);
    Lua::Libraries::luaopen_thread(_G);

    return 0;
}

void Plugin::LoadLua()
{
    lua_cpcall(_G.get(), luaopen_plugin, nullptr);

    lua_createtable(_G.get(), 0, 1);
    lua_setfield(_G.get(), LUA_REGISTRYINDEX, SOURCELUA_SCRIPT_CACHE_KEY);

    lua_createtable(_G.get(), 0, 1);
    lua_setfield(_G.get(), LUA_REGISTRYINDEX, SOURCELUA_SCHEDULER_CACHE_KEY);

    lua_createtable(_G.get(), 0, 1);
    lua_setfield(_G.get(), LUA_REGISTRYINDEX, SOURCELUA_EVENT_CACHE_KEY);
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
