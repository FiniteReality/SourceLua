#ifndef _scheduler_hpp_
#define _scheduler_hpp_

#include <memory>

#include <thread/task.hpp>

namespace SourceLua
{
namespace Threading
{
namespace Scheduler
{

void EnqueueTask(std::unique_ptr<Task> task);

void Start(lua_State* L);
void Stop();
bool IsRunning() noexcept;

}
}
}

#endif /* _scheduler_hpp_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
