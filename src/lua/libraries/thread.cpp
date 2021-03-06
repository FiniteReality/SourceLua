#include <utility>

#include <lua/libraries/thread.hpp>
#include <thread/scheduler.hpp>

using namespace SourceLua::Lua;
using namespace SourceLua::Threading;

int Sleep(lua_State* L)
{
    double delay = luaL_checknumber(L, 1);

    auto task = CreateDelayedTask(L, delay * 1000);
    Scheduler::EnqueueTask(std::move(task));
    return lua_yield(L, 0);
}

int Spawn(lua_State* L)
{
    luaL_checklfunction(L, 1);

    // Create a new thread and move the function to it
    lua_State* T = lua_newthread(L);
    lua_pushvalue(L, 1);
    lua_xmove(L, T, 1);

    auto task = CreateDelayedTask(T, 0);
    Scheduler::EnqueueTask(std::move(task));
    return 0;
}

static const luaL_Reg thread_funcs[] =
{
    {"sleep", Sleep},
    {"spawn", Spawn},
    {nullptr, nullptr}
};

int Libraries::luaopen_thread(lua_State* L)
{
    luaL_register(L, "thread", thread_funcs);
    return 1;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
