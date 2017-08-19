/*
 * Lua abstractions
 */
#include <stdexcept>

#include <common/logging.hpp>
#include <lua/lua.hpp>
#include <thread/scheduler.hpp>

namespace SourceLua
{
namespace Lua
{

Script::Script(lua_State* L)
    : _L(L), _name("Unknown")
{
    if (_L == nullptr)
        throw new std::runtime_error("L must not be null");

    _T = lua_newthread(_L);

    if (_T == nullptr)
        throw new std::runtime_error("Could not initialize Lua thread");

    // TODO: do we need to remove these objects on script destruction?
    // Will we ever have any problems with Lua threads continuing after their
    // Script has been deallocated?

    lua_getfield(_L, LUA_REGISTRYINDEX, SOURCELUA_SCRIPT_KEY);
    lua_pushvalue(_L, -2);
    lua_pushlightuserdata(_L, this);
    lua_settable(_L, -3);

    // Pop the thread and the table from the parent stack
    lua_pop(_L, 2);
}

Script::Script(lua_State* L, const char* name)
    : Script(L)
{
    _name = name;
}

void Script::Run(const char* code)
{
    Run(code, strlen(code));
}
void Script::Run(const char* code, size_t length)
{
    int err = luaL_loadbufferx(_T, code, length, _name, "t");

    const char* message;

    if (err != 0 && lua_isstring(_T, -1))
    {
        message = lua_tostring(_T, -1);
    }

    switch (err)
    {
    case 0: // No error
    {
        lua_getfield(_T, LUA_REGISTRYINDEX, SOURCELUA_SCHEDULER_KEY);
        auto scheduler = static_cast<Scheduling::Scheduler*>(
            lua_touserdata(_T, -1));

        lua_pop(_T, 1); // ensure the function is at the top of the stack

        scheduler->EnqueueCoroutine(_T, -1, 0);
        break;
    }
    case LUA_ERRSYNTAX:
        LogMessage<LogLevel::Error>(
            "Failed to load Lua code for script %s: %s",
            _name,
            message);
        break;
    default:
        if (message != nullptr)
            LogMessage<LogLevel::Warning>(
                "Unknown error occured when loading code for script %s: %s",
                _name, message);
        else
            LogMessage<LogLevel::Error>(
                "Unknown error occured while loading code for script %s",
                _name);
        break;
    }
}

}
}