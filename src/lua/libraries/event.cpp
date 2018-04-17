#include <stdexcept>
#include <tbb/concurrent_unordered_map.h>

#include <lua/objects/common.hpp>
#include <lua/libraries/event.hpp>

#include <plugin/plugin.hpp>

using namespace SourceLua::Lua;

using LuaEvent = Objects::ClassDefinition<Event>;
using EventRegistry = tbb::concurrent_unordered_map<std::string, Event>;

static EventRegistry event_map;

int GetEvent(lua_State* L)
{
    const char* name = luaL_checkstring(L, 1);

    Event& event = Libraries::GetEvent(name);

    LuaEvent::PushValue(L, &event);

    return 1;
}

int SubscribeEvent(lua_State* L)
{
    const char* name = luaL_checkstring(L, 1);
    auto* plugin = SourceLua::Plugin::GetActiveInstance();
    auto* eventManager = plugin->GetEventManager();

    if (!eventManager->FindListener(plugin, name))
        eventManager->AddListener(plugin, name, true);

    return 0;
}

static luaL_Reg event_funcs[] =
{
    {"get", GetEvent},
    {"subscribe", SubscribeEvent},
    {nullptr, nullptr}
};


int Libraries::luaopen_event(lua_State* L)
{
    luaL_register(L, "event", event_funcs);
    return 1;
}

Event& Libraries::GetEvent(const std::string name)
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
