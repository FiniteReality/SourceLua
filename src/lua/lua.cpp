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

    // Push the table now so that we don't have to re-order the stack
    lua_getfield(_L, LUA_REGISTRYINDEX, SOURCELUA_SCRIPT_KEY);
    _T = lua_newthread(_L);

    if (_T == nullptr)
        throw new std::runtime_error("Could not initialize Lua thread");

    // Reference the thread to make sure it doesn't get GC'd
    thread_ref = luaL_ref(_L, -2);
    lua_pop(_L, 1);
}

Script::Script(lua_State* L, const char* name)
    : Script(L)
{
    _name = name;
}

Script::~Script()
{
    // Remove the reference to allow GC to occur
    lua_getfield(_L, LUA_REGISTRYINDEX, SOURCELUA_SCRIPT_KEY);
    luaL_unref(_L, -1, thread_ref);
    lua_pop(_L, 1);
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

        lua_pop(_T, 1);

        scheduler->EnqueueCoroutine(_T, 0);
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
