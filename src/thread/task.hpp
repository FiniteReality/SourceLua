#ifndef _task_hpp_
#define _task_hpp_

#include <cstdint>
#include <functional>
#include <memory>

#include <common/luajit.hpp>

namespace SourceLua
{
namespace Threading
{

struct Task
{
    std::function<bool(Task*, int64_t)> ready;
    std::function<void(Task* task, lua_State* L)> prepare;
    std::function<int(lua_State*)> pushArgs;
    int thread_reference;
    int64_t enqueued_at;
};

std::unique_ptr<Task> CreateDelayedTask(lua_State* L, int delay_ms) noexcept;
std::unique_ptr<Task> CreateEventedTask(int func_ref,
    std::function<int(lua_State*)> pushArgs) noexcept;

}
}

#endif /* _task_hpp_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
