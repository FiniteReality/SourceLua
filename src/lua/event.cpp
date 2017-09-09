#include <common/logging.hpp>
#include <lua/event.hpp>
#include <lua/libraries/event.hpp>
#include <thread/task.hpp>
#include <thread/scheduler.hpp>

using namespace SourceLua::Lua;

Event::Event(lua_State* L, const std::string _name)
    : name(_name), _connections(), _L(L)
{
    Libraries::register_event(name, this);
}

Event::~Event()
{
    Libraries::deregister_event(name);
}

void Event::Fire(std::function<int(lua_State*)> pushArgs)
{
    for (int connection : _connections)
    {
        auto task = Threading::CreateEventedTask(connection, pushArgs);
        Threading::Scheduler::EnqueueTask(std::move(task));
    }
}

int Event::Connect(lua_State* L)
{
    // N.B. function is at top of stack

    lua_getfield(L, LUA_REGISTRYINDEX, SOURCELUA_EVENT_CACHE_KEY);
    lua_pushvalue(L, -2);
    int ref = luaL_ref(L, -2);

    lua_pop(L, 1);

    _connections.insert(ref);
    return ref;
}

bool Event::Disconnect(int ref)
{
    return _connections.erase(ref) > 0;
}


// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
