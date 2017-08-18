/*
 * Lua abstractions
 */
#include <stdexcept>

#include <common/logging.hpp>
#include <lua/lua.hpp>

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
}

Script::Script(lua_State* L, const char* name)
    : Script(L)
{
    _name = name;
}

void Script::PushString(const char* value)
{
    lua_pushstring(_T, value);
}

void Script::PushString(const char* value, size_t length)
{
    lua_pushlstring(_T, value, length);
}

void Script::MoveFromParent(int amount)
{
    lua_xmove(_L, _T, amount);
}
void Script::MoveToParent(int amount)
{
    lua_xmove(_T, _L, amount);
}

void Script::Resume(uint64_t delta_millis)
{
    lua_pushnumber(_T, delta_millis / 1000);
    lua_resume(_T, 1);
}

void Script::Run(const char* code)
{
    Run(code, strlen(code));
}
void Script::Run(const char* code, size_t length)
{
    int err = luaL_loadbufferx(_T, code, length, _name, "t") && lua_pcall(_T, 0, LUA_MULTRET, 0);

    switch (err)
    {
        case (LUA_ERRSYNTAX):
        {
            const char* message = lua_tostring(_T, -1);
            LogMessage<LogLevel::Error>(
                "Failed to load Lua code for script %s: %s",
                _name,
                message);
            break;
        }
        case (LUA_ERRRUN):
        {
            const char* message = lua_tostring(_T, -1);
            LogMessage<LogLevel::Error>(
                "Lua script %s experienced an error: %s",
                _name,
                message);
            break;
        }
        case (LUA_ERRMEM):
            LogMessage<LogLevel::Error>(
                "Memory allocation error occured in script %s",
                _name);
            break;
        case (LUA_ERRERR):
        default:
            LogMessage<LogLevel::Warning>(
                "Unknown error occured when loading code for script %s",
                _name);
            break;
    }
}

}
}