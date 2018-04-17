#include <lua/event.hpp>

#include "common.hpp"

using namespace SourceLua::Lua;

struct Connection
{
    Event* event;
    int ref;
};

using LuaEvent = Objects::ClassDefinition<Event>;
using LuaConnection = Objects::ClassDefinition<Connection>;

int EventEqual(lua_State* L)
{
    Event* lhs = LuaEvent::CheckValue(L, 1);
    Event* rhs = LuaEvent::CheckValue(L, 2);

    lua_pushboolean(L, (lhs == rhs) ? 1 : 0);
    return 1;
}

int EventTostring(lua_State* L)
{
    Event* event = LuaEvent::CheckValue(L, 1);
    lua_pushfstring(L, "Event '%s'", event->name().c_str());
    return 1;
}

int ConnectionTostring(lua_State* L)
{
    Connection* connection = LuaConnection::CheckValue(L, 1);
    lua_pushfstring(L, "Connection for event '%s'",
        connection->event->name().c_str());
    return 1;
}

int ConnectionDeallocate(lua_State* L)
{
    Connection* connection = LuaConnection::CheckValue(L, 1);
    delete connection;
    return 0;
}


int ConnectEvent(lua_State* L)
{
    Event* event = LuaEvent::CheckValue(L, 1);
    Connection* connection = new Connection;

    connection->event = event;
    connection->ref = event->Connect(L);

    LuaConnection::PushValue(L, connection);

    return 1;
}

int DisconnectEvent(lua_State* L)
{
    Connection* connection = LuaConnection::CheckValue(L, 1);

    bool success = connection->event->Disconnect(L, connection->ref);
    lua_pushboolean(L, success ? 1 : 0);
    return 1;
}

DEFINE_CLASS_METAMETHODS(Event,
{
    static luaL_Reg event_metamethods[] =
    {
        /*
            * Getting an event object returns a new wrapper each time, so we overwrite
            * __eq to check the underlying event object instead
            */
        {"__eq", EventEqual},
        {"__tostring", EventTostring},
        {nullptr, nullptr}
    };
    luaL_register(L, nullptr, event_metamethods);
})

DEFINE_CLASS_METHODS(Event,
{
    static luaL_Reg event_methods[] =
    {
        {"connect", ConnectEvent},
        {nullptr, nullptr}
    };
    luaL_register(L, nullptr, event_methods);
})

DEFINE_CLASS_SUBTYPES(Event,
{
    LuaConnection::RegisterType(L);
})

DEFINE_CLASS(Event)

DEFINE_CLASS_METAMETHODS(Connection,
{
    static luaL_Reg connection_metamethods[] =
    {
        {"__tostring", ConnectionTostring},
        {"__gc", ConnectionDeallocate},
        {nullptr, nullptr}
    };
    luaL_register(L, nullptr, connection_metamethods);
})

DEFINE_CLASS_METHODS(Connection,
{
    static luaL_Reg connection_methods[] =
    {
        {"disconnect", DisconnectEvent},
        {nullptr, nullptr}
    };
    luaL_register(L, nullptr, connection_methods);
})

DEFINE_CLASS(Connection)

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
