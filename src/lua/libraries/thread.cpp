#include <lua/libraries/thread.hpp>
#include <plugin/plugin.hpp>
#include <thread/scheduler.hpp>

namespace SourceLua
{
namespace Lua
{
namespace Threading
{
    int Sleep(lua_State* L)
    {
        double delay = luaL_checknumber(L, 1);

        lua_getfield(L, LUA_REGISTRYINDEX, SOURCELUA_SCHEDULER_KEY);
        auto scheduler = static_cast<Scheduling::Scheduler*>(
            lua_touserdata(L, -1));

        lua_pop(L, 1);

        lua_pushthread(L);
        scheduler->EnqueueCoroutine(L, -1, (unsigned int)(delay * 1000));

        return lua_yield(L, 0);
    }

    int Spawn(lua_State* L)
    {
        luaL_argcheck(L, lua_isfunction(L, 1) && !lua_iscfunction(L, 1), 1,
            "Lua function expected");

        lua_getfield(L, LUA_REGISTRYINDEX, SOURCELUA_SCHEDULER_KEY);
        auto scheduler = static_cast<Scheduling::Scheduler*>(
            lua_touserdata(L, -1));

        lua_pop(L, 1);

        // Create a new thread and move the function to it
        lua_State* T = lua_newthread(L);
        lua_pushvalue(L, 1);
        lua_xmove(L, T, 1);

        scheduler->EnqueueCoroutine(T, -1, 0);
        return 1; // Return the thread so users can track it
    }

    static const luaL_Reg thread_funcs[] =
    {
        {"sleep", Sleep},
        {"spawn", Spawn},
        {NULL, NULL}
    };

    int luaopen_thread(lua_State* L)
    {
        luaL_register(L, "thread", thread_funcs);
        return 1;
    }
}
}
}