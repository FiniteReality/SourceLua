#include <lua/event.hpp>
#include <lua/objects/common.hpp>
#include <lua/objects/event.hpp>

using namespace SourceLua::Lua;

struct EventWrapper
{
    Event* event;
};

struct ConnectionWrapper
{
    Event* event;
    int ref;
};

void PushEvent(lua_State* L, Event* event)
{
    EventWrapper* wrapper = newUData(L, EventWrapper);

    wrapper->event = event;
    luaL_getmetatable(L, SOURCELUA_EVENT_OBJECT_KEY);
    lua_setmetatable(L, -2);
}

void PushConnection(lua_State* L, Event* event, int ref)
{
    ConnectionWrapper* wrapper = newUData(L, ConnectionWrapper);

    wrapper->event = event;
    wrapper->ref = ref;

    luaL_getmetatable(L, SOURCELUA_CONNECTION_OBJECT_KEY);
    lua_setmetatable(L, -2);
}


int EventEqual(lua_State* L)
{
    EventWrapper* lhs =
        checkUData(L, 1, SOURCELUA_EVENT_OBJECT_KEY, EventWrapper);
    EventWrapper* rhs =
        checkUData(L, 2, SOURCELUA_EVENT_OBJECT_KEY, EventWrapper);

    lua_pushboolean(L, lhs->event == rhs->event ? 1 : 0);
    return 1;
}

int EventTostring(lua_State* L)
{
    EventWrapper* wrapper =
        checkUData(L, 1, SOURCELUA_EVENT_OBJECT_KEY, EventWrapper);
    lua_pushfstring(L, "Event '%s'", wrapper->event->name.c_str());
    return 1;
}

int ConnectionTostring(lua_State* L)
{
    ConnectionWrapper* wrapper =
        checkUData(L, 1, SOURCELUA_CONNECTION_OBJECT_KEY, ConnectionWrapper);

    lua_pushfstring(L, "Connection for event '%s'",
                    wrapper->event->name.c_str());
    return 1;
}


int ConnectEvent(lua_State* L)
{
    EventWrapper* wrapper =
        checkUData(L, 1, SOURCELUA_EVENT_OBJECT_KEY, EventWrapper);

    int ref = wrapper->event->Connect(L);
    PushConnection(L, wrapper->event, ref);
    return 1;
}

int DisconnectEvent(lua_State* L)
{
    ConnectionWrapper* wrapper =
        checkUData(L, 1, SOURCELUA_CONNECTION_OBJECT_KEY, ConnectionWrapper);

    bool success = wrapper->event->Disconnect(wrapper->ref);
    lua_pushboolean(L, success ? 1 : 0);
    return 1;
}


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
static luaL_Reg connection_metamethods[] =
{
    {"__tostring", ConnectionTostring},
    {nullptr, nullptr}
};

static luaL_Reg event_methods[] =
{
    {"connect", ConnectEvent},
    {nullptr, nullptr}
};
static luaL_Reg connection_methods[] =
{
    {"disconnect", DisconnectEvent},
    {nullptr, nullptr}
};


int Objects::WrapEvent(lua_State* L, Event* event)
{
    PushEvent(L, event);
    return 1;
}

int Objects::luaopen_event_wrapper(lua_State* L)
{
    luaL_newmetatable(L, SOURCELUA_EVENT_OBJECT_KEY);
    luaL_register(L, nullptr, event_metamethods);
    lua_pushliteral(L, "This metatable is locked");
    lua_setfield(L, -2, "__metatable");

    lua_newtable(L);
    luaL_register(L, nullptr, event_methods);
    lua_setfield(L, -2, "__index");

    luaL_newmetatable(L, SOURCELUA_CONNECTION_OBJECT_KEY);
    luaL_register(L, nullptr, connection_metamethods);
    lua_pushliteral(L, "This metatable is locked");
    lua_setfield(L, -2, "__metatable");

    lua_newtable(L);
    luaL_register(L, nullptr, connection_methods);
    lua_setfield(L, -2, "__index");

    return 0;
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
