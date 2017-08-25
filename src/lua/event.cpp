#include <common/logging.hpp>
#include <lua/event.hpp>
#include <lua/libraries/event.hpp>

/*
 * Event implementation
 */

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


Event::~Event()
{
    for (std::unique_ptr<Connection>& connection : connections)
    {
        // Remove any referenced threads so that they can be GC'd
        lua_getfield(L, LUA_REGISTRYINDEX, SOURCELUA_EVENT_CACHE_KEY);
        luaL_unref(L, -1, connection->ref);
    }
}

void Event::Fire(std::function<int (lua_State*)> pushArgs)
{
    LogMessage<LogLevel::Debug>("Firing event %s", name);

    for (std::unique_ptr<Connection>& connection : connections)
    {
        int numResults = pushArgs(connection->L);

        // HACK: will this cause segfaults with multiple threads running?
        lua_pcall(L, numResults, 0, 0);

        // TODO: check status and log errors if necessary
    }
}

void Event::Connect(lua_State* L)
{
    // N.B. function is at top of stack

    lua_getfield(L, LUA_REGISTRYINDEX, SOURCELUA_EVENT_CACHE_KEY);
    lua_pushvalue(L, -2);
    int ref = luaL_ref(L, -1);


    Connection connection;
    connection.L = L;
    connection.ref = ref;

    connections.push_back(
        std::make_unique<Connection>(std::move(connection)));
}


}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
