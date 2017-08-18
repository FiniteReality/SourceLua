#ifndef _scheduler_hpp_
#define _scheduler_hpp_

/*
 * Schedules Lua tasks to be ran on a game tick
 */

#include <memory>
#include <vector>

#include <lua/lua.hpp>

namespace SourceLua
{
namespace Scheduling
{
class Scheduler
{
    public:
        void EnqueueCoroutine(Lua::Script* script,
            int idx, unsigned int delay_msec);

        void Tick();

    private:
        struct TaskInfo
        {
            TaskInfo(Lua::Script* script, int idx,
                unsigned int resume_at);
            TaskInfo(const TaskInfo&) = default;
            TaskInfo& operator=(const TaskInfo&) = default;
            TaskInfo(TaskInfo&&) = default;
            TaskInfo& operator=(TaskInfo&&) = default;
            ~TaskInfo() = default;

            Lua::Script* script;
            int idx;
            unsigned int resume_at_millis;
        };

        std::vector<std::unique_ptr<TaskInfo>> tasks;
};
}
}

#endif /* _scheduler_hpp_ */