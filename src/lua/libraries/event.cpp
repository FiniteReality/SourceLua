#include <lua/libraries/event.hpp>

#include <tbb/concurrent_unordered_map.h>

namespace SourceLua
{
namespace Lua
{
namespace Libraries
{

struct EventWrapper
{
    Event* event;
};

static tbb::concurrent_unordered_map<std::string, Event*>
event_registry;


int PushWrapper(lua_State* L, Event* event)
{
    EventWrapper* ud = static_cast<EventWrapper*>(
        lua_newuserdata(L, sizeof(EventWrapper))
    );

    ud->event = event;

    luaL_getmetatable(L, SOURCELUA_EVENT_KEY);
    lua_setmetatable(L, -2);

    return 1;
}

int wrapper_connect(lua_State* L)
{
    EventWrapper* ud = static_cast<EventWrapper*>(
        luaL_checkudata(L, 1, SOURCELUA_EVENT_KEY)
    );

    luaL_checklfunction(L, 2);

    Event* event = ud->event;
    event->Connect(L);

    return 0;
}

int wrapper_index(lua_State* L)
{
    luaL_checkudata(L, 1, SOURCELUA_EVENT_KEY);

    // TODO: this entire thing should be replaced with a switch-based system
    const char* request = luaL_checkstring(L, 2);

    if (strcmp(request, "connect") == 0)
    {
        lua_pushcfunction(L, wrapper_connect);
        return 1;
    }

    return luaL_error(L, "Unknown member %s", request);
}

static luaL_Reg event_metafuncs[] =
{
    {"__index", wrapper_index},
    {nullptr, nullptr}
};


int GetEvent(lua_State* L)
{
    const char* name = luaL_checkstring(L, 1);
    auto itr = event_registry.find(std::string {name});

    return PushWrapper(L, itr->second);
}

static luaL_Reg event_funcs[] =
{
    {"getEvent", GetEvent},
    {nullptr, nullptr}
};


int luaopen_event(lua_State* L)
{
    luaL_newmetatable(L, SOURCELUA_EVENT_KEY);

    luaL_register(L, nullptr, event_metafuncs);

    lua_pushliteral(L, "__metatable");
    lua_pushliteral(L, "This metatable is locked");
    lua_settable(L, -3);

    luaL_register(L, "event", event_funcs);

    return 1;
}

void register_event(const char* name, Event* event)
{
    event_registry.emplace(std::string {name}, event);
}

}
}
}


// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
