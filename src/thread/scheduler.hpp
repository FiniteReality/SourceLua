#ifndef _scheduler_hpp_
#define _scheduler_hpp_

/*
 * Schedules Lua tasks to be ran on a game tick
 */

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <tbb/concurrent_priority_queue.h>
#include <thread>

#include <lua/lua.hpp>

namespace SourceLua
{
namespace Scheduling
{
class Scheduler
{
    public:
        void EnqueueCoroutine(lua_State* L, int idx,
            unsigned int delay_msec);

        void Tick();

        void Start();
        void Stop();
    private:
        static void TickThread(Scheduler* info);

        struct TaskInfo
        {
            TaskInfo(lua_State* L, int idx,
                uint64_t resume_at, uint64_t now);
            TaskInfo(const TaskInfo&) = default;
            TaskInfo& operator=(const TaskInfo&) = default;
            TaskInfo(TaskInfo&&) = default;
            TaskInfo& operator=(TaskInfo&&) = default;
            ~TaskInfo() = default;

            lua_State* state;
            int idx;
            uint64_t resume_at_millis;
            uint64_t enqueued_at;

            friend bool operator<(const TaskInfo& lhs, const TaskInfo& rhs)
            {
                return lhs.resume_at_millis
                     < rhs.resume_at_millis;
            }
        };

        tbb::concurrent_priority_queue<std::unique_ptr<TaskInfo>> tasks;

        std::atomic_bool running;

        std::thread thread;
        std::mutex resume_lock;
        std::condition_variable thread_available;
};
}
}

#endif /* _scheduler_hpp_ */