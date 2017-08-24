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
        void EnqueueCoroutine(lua_State* L, unsigned int delay_msec);

        void Tick();

        void Start();
        void Stop();
    private:
        static void TickThread(Scheduler* info);

        struct TaskInfo
        {
            TaskInfo(lua_State* L, int ref, int64_t resume_at, int64_t now);
            TaskInfo(const TaskInfo&) = default;
            TaskInfo& operator=(const TaskInfo&) = default;
            TaskInfo(TaskInfo&&) = default;
            TaskInfo& operator=(TaskInfo&&) = default;
            ~TaskInfo() = default;

            lua_State* state;
            int ref;
            int64_t resume_at;
            int64_t enqueued_at;

            friend inline bool operator< (const TaskInfo& lhs,
                const TaskInfo& rhs)
            {
                return std::tie(lhs.resume_at, lhs.enqueued_at) <
                       std::tie(rhs.resume_at, rhs.enqueued_at);
            }
        };

        tbb::concurrent_priority_queue<std::unique_ptr<TaskInfo>> tasks;

        std::atomic_bool run;

        std::thread thread;
        std::mutex resume_lock;
        std::condition_variable thread_available;
};
}

}

#endif /* _scheduler_hpp_ */
