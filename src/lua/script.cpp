#include <stdexcept>

#include <common/logging.hpp>
#include <lua/error_handler.hpp>
#include <lua/script.hpp>
#include <thread/scheduler.hpp>

namespace SourceLua
{
namespace Lua
{

Script::Script(lua_State* L)
    : _L{L}, _name{"=unknown"}
{
    if (_L == nullptr)
        throw new std::runtime_error("L must not be null");

    // Push the table now so that we don't have to re-order the stack
    lua_getfield(_L, LUA_REGISTRYINDEX, SOURCELUA_SCRIPT_CACHE_KEY);
    _T = lua_newthread(_L);

    if (_T == nullptr)
        throw new std::runtime_error("Could not initialize Lua thread");

    thread_ref = luaL_ref(L, -2);

    lua_pop(_L, 1);
}

Script::Script(lua_State* L, const char* name)
    : Script(L)
{
    _name.replace(1, _name.size(), name);
}

Script::~Script()
{
    // Remove the reference to allow GC to occur
    lua_getfield(_L, LUA_REGISTRYINDEX, SOURCELUA_SCRIPT_CACHE_KEY);

    luaL_unref(_L, -1, thread_ref);

    lua_pop(_L, 1);
}

void Script::Run(const char* code)
{
    Run(code, strlen(code));
}
void Script::Run(const char* code, size_t length)
{
    int err = luaL_loadbufferx(_T, code, length, _name.c_str(), "t");

    if (err == LUA_OK)
    {
        auto task = Threading::CreateDelayedTask(_T, 0);
        Threading::Scheduler::EnqueueTask(std::move(task));
    }
    else
    {
        Errors::HandleError(_T, err);
    }
}

const char* Script::name() const
{
    return _name.substr(1).c_str();
}

}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
