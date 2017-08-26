#include <common/logging.hpp>
#include <lua/event.hpp>
#include <lua/libraries/event.hpp>
#include <thread/task.hpp>
#include <thread/scheduler.hpp>

namespace SourceLua
{
namespace Lua
{

Event::Event(lua_State* _L)
    : connections(), L(_L), name("Unknown")
{ }
Event::Event::Event(lua_State* L, const char* _name)
    : Event(L)
{
    name = _name;
    Libraries::register_event(_name, this);
}

void Event::Fire(std::function<int(lua_State*)> pushArgs)
{
    LogMessage<LogLevel::Debug>("Firing event %s", name);

    for (Connection& connection : connections)
    {
        auto task = Threading::CreateEventedTask(connection.ref, pushArgs);
        Threading::Scheduler::EnqueueTask(std::move(task));
    }
}

void Event::Connect(lua_State* L)
{
    // N.B. function is at top of stack

    lua_getfield(L, LUA_REGISTRYINDEX, SOURCELUA_EVENT_CACHE_KEY);
    lua_pushvalue(L, -2);
    int ref = luaL_ref(L, -2);

    lua_pop(L, 1);

    Connection connection;
    connection.ref = ref;
    connections.emplace_back(std::move(connection));
}


}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
