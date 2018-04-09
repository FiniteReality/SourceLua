#include <stdexcept>
#include <tbb/concurrent_unordered_map.h>

#include <lua/objects/common.hpp>
#include <lua/libraries/event.hpp>

using namespace SourceLua::Lua;

using LuaEvent = Objects::ClassDefinition<Event>;

static tbb::concurrent_unordered_map<std::string, Event*> event_registry;

int GetEvent(lua_State* L)
{
    const char* name = luaL_checkstring(L, 1);

    auto itr = event_registry.find(name);

    if (itr != event_registry.end())
    {
        LuaEvent::PushValue(L, itr->second);
        return 1;
    }

    return luaL_error(L, "No event with the name '%s'", name);
}

static luaL_Reg event_funcs[] =
{
    {"get", GetEvent},
    {nullptr, nullptr}
};


int Libraries::luaopen_event(lua_State* L)
{
    LuaEvent::RegisterType(L);
    luaL_register(L, "event", event_funcs);
    return 1;
}

void Libraries::register_event(const std::string name, Event* event)
{
    bool success;
    std::tie(std::ignore, success) = event_registry
        .emplace(name, event);

    if (!success)
        throw std::runtime_error("an event with this name already exists");
}

void Libraries::deregister_event(const std::string name)
{
    /* N.B. deregister_event is only called in Event's destructor in Plugin,
     * this should be fine, assuming only one thread tries to destruct the
     * plugin.
     */
    event_registry.unsafe_erase(name);
}


// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
