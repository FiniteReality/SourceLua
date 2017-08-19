/*
 * Thread scheduler implementation
 */

#include <algorithm>
#include <chrono>

#include <common/logging.hpp>
#include <info.hpp>
#include <thread/scheduler.hpp>

#define GetCurrentTime(duration) \
    std::chrono::duration_cast<duration>( \
        std::chrono::high_resolution_clock::now() \
        .time_since_epoch() \
    ).count()

namespace SourceLua
{
namespace Scheduling
{
Scheduler::TaskInfo::TaskInfo(lua_State* L, int _idx,
    uint64_t _resume_at_millis, uint64_t now)
        : state(L), idx(_idx), resume_at_millis(_resume_at_millis),
          enqueued_at(now)
{ }

void Scheduler::EnqueueCoroutine(lua_State* L, int idx,
    unsigned int delay_msec)
{
    uint64_t now = GetCurrentTime(std::chrono::milliseconds);
    uint64_t resume_at = now + delay_msec;

    tasks.push(std::make_unique<TaskInfo>(L, idx, resume_at, now));
}

void Scheduler::Tick()
{
    thread_available.notify_all();
}

void Scheduler::Start()
{
    LogMessage<LogLevel::Debug>("Starting thread scheduler");
    running = true;

    thread = std::thread(TickThread, this);

}

void Scheduler::Stop()
{
    LogMessage<LogLevel::Debug>("Stopping thread scheduler");
    running = false;
    thread_available.notify_all();

    thread.join();
}

void Scheduler::TickThread(Scheduler* info)
{
    while (info->running)
    {
        std::unique_lock<std::mutex> lk(info->resume_lock);
        info->thread_available.wait(lk,
            [=]{
                return !info->running || info->tasks.size() > 0;
            });

        LogMessage<LogLevel::Debug>(
            "There are %d tasks"
        )

        uint64_t now = GetCurrentTime(std::chrono::milliseconds);

        std::unique_ptr<TaskInfo> task;
        if (info->tasks.try_pop(task))
        {
            LogMessage<LogLevel::Debug>(
                "Task resumes at %ull, now = %ull",
                task->resume_at_millis,
                now);

            if (now >= task->resume_at_millis)
            {
                auto L = task->state;
                int status = lua_status(L);

                LogMessage<LogLevel::Debug>(
                    "Task with status %d ready to resume", status);
                if (status == LUA_YIELD || status == 0)
                {
                    lua_pushvalue(L, task->idx);
                    lua_pushnumber(L, (now - task->enqueued_at) / 1000.0);
                    lua_pushnumber(L, now / 1000.0);
                    int status = lua_resume(L, 2);

                    LogMessage<LogLevel::Debug>("Task resume gave status %d",
                        status);

                    if (status != 0 && status != LUA_YIELD)
                    {
                        if (lua_isstring(L, -1))
                        {
                            const char* message = lua_tostring(L, -1);
                            LogMessage<LogLevel::Error>(
                                "Lua task experienced an error: %s",
                                message);
                        }
                    }
                }
            }
            else
            {
                // Add 1ms to the resume time so that other tasks have a
                // chance to be run concurrently
                task->resume_at_millis += 1;
                info->tasks.push(std::move(task));
            }
        }
    }
}

}
}