#include <common/logging.hpp>
#include <thread/task.hpp>

using namespace SourceLua;
using namespace std;

unique_ptr<Threading::Task> Threading::CreateDelayedTask(lua_State* L,
    int delay_ms) noexcept
{
    lua_getfield(L, LUA_REGISTRYINDEX, SOURCELUA_SCHEDULER_CACHE_KEY);
    lua_pushthread(L);
    int ref = luaL_ref(L, -2);
    lua_pop(L, 1);

    unique_ptr<Threading::Task> task = std::make_unique<Threading::Task>();
    task->thread_reference = ref;
    task->ready = [delay_ms](Threading::Task* task, int64_t now){
        return now > (task->enqueued_at + delay_ms);
    };

    return task;
}

unique_ptr<Threading::Task> Threading::CreateEventedTask(int func_ref,
    std::function<int(lua_State*)> pushArgs) noexcept
{
    unique_ptr<Threading::Task> task = std::make_unique<Threading::Task>();
    task->pushArgs = pushArgs;
    task->prepare = [func_ref](Threading::Task* task, lua_State* L){
        lua_getfield(L, LUA_REGISTRYINDEX, SOURCELUA_SCHEDULER_CACHE_KEY);

        lua_State* T = lua_newthread(L);
        lua_getfield(T, LUA_REGISTRYINDEX, SOURCELUA_EVENT_CACHE_KEY);
        lua_rawgeti(T, -1, func_ref);

        int ref = luaL_ref(L, -2);
        lua_pop(L, 1);

        task->thread_reference = ref;
    };

    return task;
}
