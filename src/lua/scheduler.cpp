/*
 * Thread scheduler implementation
 */

#include <algorithm>
#include <chrono>

#include <common/logging.hpp>
#include <lua/scheduler.hpp>

#define GetCurrentTime(duration) \
    std::chrono::duration_cast<duration>( \
        std::chrono::high_resolution_clock::now() \
        .time_since_epoch() \
    ).count()

namespace SourceLua
{
namespace Lua
{

Scheduler::TaskInfo::TaskInfo(lua_State* L, int _ref,
                                int64_t _resume_at_millis, int64_t now)
: state(L), ref(_ref), resume_at(_resume_at_millis),
enqueued_at(now)
{ }

void Scheduler::EnqueueCoroutine(lua_State* L, unsigned int delay_msec)
{
    int64_t now = GetCurrentTime(std::chrono::milliseconds);
    int64_t resume_at = now + delay_msec;

    // Cache the lua state in Lua so that it doesn't get GC'd
    lua_getfield(L, LUA_REGISTRYINDEX, SOURCELUA_SCHEDULER_CACHE_KEY);
    lua_pushthread(L);
    int ref = luaL_ref(L, -2);

    lua_pop(L, 1);

    tasks.emplace(new Scheduler::TaskInfo(L, ref, resume_at, now));
}

void Scheduler::Tick()
{
    thread_available.notify_all();
}

void Scheduler::Start()
{
    LogMessage<LogLevel::Debug>("Starting thread scheduler");
    run = true;

    thread = std::thread(TickThread, this);
}

void Scheduler::Stop()
{
    LogMessage<LogLevel::Debug>("Stopping thread scheduler");
    run = false;

    thread_available.notify_all();
    thread.join();
}

void Scheduler::TickThread(Scheduler* scheduler)
{
    while (scheduler->run)
    {
        std::unique_lock<std::mutex> lk(scheduler->resume_lock);
        scheduler->thread_available.wait(lk);

        // If we came out of the lock because Stop() was called, we don't need
        // to process anything.
        if (!scheduler->run)
        {
            break;
        }

        int64_t now = GetCurrentTime(std::chrono::milliseconds);

        std::unique_ptr<TaskInfo> task;

        if (!scheduler->tasks.try_pop(task))
        {
            continue;
        }

        if (now < task->resume_at)
        {
            //task->resume_at++;
            scheduler->tasks.push(std::move(task));
            continue;
        }

        lua_State* L = task->state;
        int status = lua_status(L);


        if (status == 0)
        {
            if (lua_gettop(L) == 0)
            {
                continue;
            }
        }
        else if (status != LUA_YIELD)
        {
            continue;
        }


        lua_pushnumber(L, (now - task->enqueued_at) / 1000.0);
        lua_pushnumber(L, now / 1000.0);
        status = lua_resume(L, 2);

        if (status != 0 && status != LUA_YIELD)
        {
            if (lua_isstring(L, -1))
            {
                // TODO: this should be moved to a dedicated error handler
                const char* message = lua_tostring(L, -1);
                LogMessage<LogLevel::Error>(
                    "Lua task experienced an error: %s",
                    message);
            }
        }

        // Un-ref the state so it can be GC'd if necessary
        lua_getfield(L, LUA_REGISTRYINDEX, SOURCELUA_SCHEDULER_CACHE_KEY);
        luaL_unref(L, -1, task->ref);
        lua_pop(L, 1);
    }
}

}
}




// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
