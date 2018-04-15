#include <atomic>
#include <condition_variable>
#include <chrono>
#include <future>
#include <memory>
#include <mutex>
#include <tbb/concurrent_queue.h>
#include <thread>

#include <common/logging.hpp>
#include <lua/error_handler.hpp>
#include <thread/scheduler.hpp>

using namespace SourceLua;
using namespace SourceLua::Lua;
using namespace SourceLua::Threading;
using namespace std;
using namespace tbb;

static concurrent_queue<unique_ptr<Task>> scheduler_tasks;
static thread scheduler_thread;
static mutex scheduler_lock;
static condition_variable scheduler_condition;
// Atomic since multiple threads can be reading and writing in parallel
static atomic_bool scheduler_running;

static void TaskThread(lua_State* L) noexcept;

void Scheduler::EnqueueTask(unique_ptr<Task> task)
{
    int64_t now =
        chrono::duration_cast<chrono::milliseconds>(
            chrono::high_resolution_clock::now()
            .time_since_epoch())
        .count();

    task->enqueued_at = now;

    scheduler_tasks.push(std::move(task));
    scheduler_condition.notify_all();
}

void Scheduler::Start(lua_State* L)
{
    scheduler_running = true;
    scheduler_thread = std::thread(TaskThread, L);
}

void Scheduler::Stop()
{
    scheduler_running = false;
    scheduler_condition.notify_all();

    // N.B. this approach spawns a new thread to do the work of waiting
    // Is there a better way to do this?
    auto future = async(launch::async, &thread::join, &scheduler_thread);

    if (future.wait_for(chrono::seconds(5)) == future_status::timeout)
    {
        throw new runtime_error("Could not abort scheduler thread");
    }
}

bool Scheduler::IsRunning() noexcept
{
    return scheduler_running;
}

static void TaskThread(lua_State* L) noexcept
{
    while (scheduler_running)
    {
        std::unique_lock<std::mutex> lk(scheduler_lock);
        scheduler_condition.wait(lk, []
        {
            return !(scheduler_running && scheduler_tasks.empty());
        });

        // If we were signaled to stop, we should return at this point.
        if (!scheduler_running)
            return;

        int64_t now =
            chrono::duration_cast<chrono::milliseconds>(
                chrono::high_resolution_clock::now()
                .time_since_epoch())
            .count();

        unique_ptr<Task> task;

        if (scheduler_tasks.try_pop(task))
        {
            if (task->ready == nullptr || task->ready(task.get(), now))
            {
                // Resume the task
                LogMessage<LogLevel::Debug>("Lua task ready to resume");

                if (task->prepare != nullptr)
                    task->prepare(task.get(), L);

                lua_getfield(L, LUA_REGISTRYINDEX, SOURCELUA_SCHEDULER_CACHE_KEY);
                lua_rawgeti(L, -1, task->thread_reference);
                lua_State* T = lua_tothread(L, -1);

                int status = lua_status(T);

                if (status == LUA_YIELD || status == LUA_OK)
                {
                    if (status == LUA_OK &&
                            lua_type(T, lua_gettop(T)) != LUA_TFUNCTION)
                    {
                        LogMessage<LogLevel::Warning>(
                            "Tried to resume an invalid task");
                        continue;
                    }

                    int argCount = 0;

                    if (task->pushArgs != nullptr)
                        argCount = task->pushArgs(T);

                    status = lua_resume(T, argCount);

                    if (status != LUA_OK && status != LUA_YIELD)
                    {
                        Errors::HandleError(T, status);
                    }

                    LogMessage<LogLevel::Debug>(
                        "lua_resume gave status %d",
                        status);

                    lua_pop(L, 1);
                    luaL_unref(L, -1, task->thread_reference);
                    lua_pop(L, 1);
                }
                else
                {
                    Errors::HandleError(L, status);
                }
            }
            else
            {
                // Make sure the task gets placed back on the queue
                scheduler_tasks.push(std::move(task));
            }
        }
    }
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
