/*
 * Thread scheduler implementation
 */

#include <algorithm>
#include <chrono>

#include <thread/scheduler.hpp>

#include <info.hpp>

#define GetCurrentTime(duration) \
    std::chrono::duration_cast<duration>( \
        std::chrono::high_resolution_clock::now() \
        .time_since_epoch() \
    ).count()

namespace SourceLua
{
namespace Scheduling
{
Scheduler::TaskInfo::TaskInfo(Lua::Script* _script, int _idx,
    unsigned int _resume_at_millis)
        : script(_script), idx(_idx), resume_at_millis(_resume_at_millis)
{ }

void Scheduler::EnqueueCoroutine(Lua::Script* script, int idx,
    unsigned int delay_msec)
{
    auto resume_at = GetCurrentTime(std::chrono::milliseconds)
        + delay_msec;
    tasks.push(std::make_unique<TaskInfo>(script, idx, resume_at));
    thread_available.notify_one();
}

void Scheduler::Tick()
{
    thread_available.notify_one();
}

void Scheduler::Start()
{
    running = true;
    scheduler_thread = std::thread(TickThread, this);
}

void Scheduler::Stop()
{
    running = false;
    if (scheduler_thread.joinable())
        scheduler_thread.join();
}

void Scheduler::TickThread(Scheduler* info)
{
    while (info->running)
    {
        std::unique_lock<std::mutex> lk(info->resume_lock);
        info->thread_available.wait(lk);

        auto now = GetCurrentTime(std::chrono::milliseconds);

        std::unique_ptr<TaskInfo> task;
        if (info->tasks.try_pop(task))
        {
            if (task->resume_at_millis > now)
            {
                task->script->PushValue(task->idx);
                task->script->Resume(now - task->resume_at_millis);
            }
            else
            {
                info->tasks.push(std::move(task));
            }
        }
    }
}

}
}