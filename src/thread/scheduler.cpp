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
            tasks.push_back(std::make_unique<TaskInfo>(script, idx, resume_at));
        }

        void Scheduler::Tick()
        {
            auto right_now = GetCurrentTime(std::chrono::milliseconds);

            for (auto& task : tasks)
            {
                if (right_now >= task->resume_at_millis)
                {
                    task->script->Resume(right_now - task->resume_at_millis);
                    tasks.erase(std::remove(tasks.begin(), tasks.end(), task), tasks.end());
                    break;
                }
            }
        }
    }
}