#include <stdexcept>
#include <tbb/concurrent_unordered_map.h>

#include <lua/objects/common.hpp>
#include <lua/libraries/event.hpp>

using namespace SourceLua::Lua;

using LuaEvent = Objects::ClassDefinition<Event>;
using EventRegistry = tbb::concurrent_unordered_map<std::string, Event>;

static EventRegistry event_map;

int GetEvent(lua_State* L)
{
    const char* name = luaL_checkstring(L, 1);

    Event& event = Libraries::get_event(name);

    LuaEvent::PushValue(L, &event);

    return 1;
}

static luaL_Reg event_funcs[] =
{
    {"get", GetEvent},
    {nullptr, nullptr}
};


int Libraries::luaopen_event(lua_State* L)
{
    luaL_register(L, "event", event_funcs);
    return 1;
}

Event& Libraries::get_event(const std::string name)
{
    EventRegistry::const_iterator iter = event_map.find(name);

    if (iter != event_map.end())
    {
        return iter->second;
    }
    else
    {
        bool success;
        std::tie(iter, success) = event_map.emplace(name, Event{name});

        if (success)
            return iter->second;
        else
            throw std::runtime_error{"Failed to construct event type"};
    }
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
