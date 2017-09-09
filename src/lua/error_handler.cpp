#include <iostream>
#include <sstream>

#include <common/logging.hpp>
#include <lua/error_handler.hpp>
#include <lua/script.hpp>

using namespace SourceLua;
using namespace SourceLua::Lua;

/*
 #define LUA_YIELD  1
 #define LUA_ERRRUN 2
 #define LUA_ERRSYNTAX  3
 #define LUA_ERRMEM 4
 #define LUA_ERRERR 5
*/

void HandleRuntimeError(lua_State* L);
void HandleSyntaxError(lua_State* L);
void HandleMemoryError(lua_State* L);
void HandleErrorHandlerError(lua_State* L);

std::string GetScriptNameFromState(lua_State *L)
{
    lua_Debug debug;
    int i = 0;
    int status = 0;

    while (i == 0 || (status != 0 && *debug.what == 'C'))
    {
        status = lua_getstack(L, i++, &debug);
        lua_getinfo(L, "S", &debug);
    }

    if (*debug.short_src == '\0')
        return "?";

    return std::string{debug.short_src};
}

void Errors::HandleError(lua_State* L, int status)
{
    switch(status)
    {
        case LUA_OK:
        case LUA_YIELD:
            LogMessage<LogLevel::Debug>(
                "Errors::HandleError called when not in error state");
            break;
        case LUA_ERRRUN:
            HandleRuntimeError(L);
            break;
        case LUA_ERRSYNTAX:
            HandleSyntaxError(L);
            break;
        case LUA_ERRMEM:
            HandleMemoryError(L);
            break;
        case LUA_ERRERR:
            HandleErrorHandlerError(L);
            break;
    }
}


void HandleRuntimeError(lua_State* L)
{
    // If the error message wasn't a string, don't do anything
    if (!lua_isstring(L, -1))
    {
        std::string script = GetScriptNameFromState(L);
        LogMessage<LogLevel::Error>(
            "Lua script '%s' experienced an unknown runtime error",
            script.c_str());
        return;
    }

    size_t length;
    const char* error = lua_tolstring(L, -1, &length);

    lua_Debug ar;
    std::ostringstream msg;

    msg.write(error, length) << '\n'
        << "Stack traceback:";


    for (int level = 0; lua_getstack(L, level, &ar); level++)
    {
        lua_getinfo(L, "Snl", &ar);

        msg << "\n\t"
            << ar.short_src << ':';

        if (ar.currentline > 0)
            msg << ar.currentline;

        if (*ar.namewhat != '\0')
        {
            msg << " in function '" << ar.name << '\'';
        }
        else
        {
            if (*ar.what == 'm')
                msg << " in main chunk";
            else if (*ar.what == 'C' || *ar.what == 't')
                msg << " ?";
            else
            {
                msg << " in function <"
                    << ar.short_src
                    << ':'
                    << ar.linedefined
                    << '>';
            }
        }
    }

    auto message = msg.str();

    LogMessage<LogLevel::Error>(message.c_str());
}

void HandleSyntaxError(lua_State* L)
{
    // If the error message wasn't a string, don't do anything
    if (!lua_isstring(L, -1))
    {
        std::string script = GetScriptNameFromState(L);
        LogMessage<LogLevel::Error>(
            "Lua script '%s' experienced an unknown syntax error",
            script.c_str());
        return;
    }

    LogMessage<LogLevel::Error>(lua_tostring(L, -1));
}

void HandleMemoryError(lua_State* L)
{
    std::string script = GetScriptNameFromState(L);
    LogMessage<LogLevel::Error>(
        "Lua script '%s' experienced a memory allocation error",
        script.c_str()
    );
}

void HandleErrorHandlerError(lua_State* L)
{
    if (!lua_isstring(L, -1))
    {
        std::string script = GetScriptNameFromState(L);
        LogMessage<LogLevel::Error>(
            "Lua script '%s' experienced an unknown runtime error",
            script.c_str());
        return;
    }

    LogMessage<LogLevel::Error>(lua_tostring(L, -1));
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
