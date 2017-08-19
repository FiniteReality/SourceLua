#ifndef _luajit_hpp_
#define _luajit_hpp_

/*
 * LuaJIT utilities
 */

#include <lua.hpp>

#include <common/version.hpp>

#ifndef LUAJIT_VERSION
#error "LuaJIT wasn't included properly"
#endif

#define SOURCELUA_SCRIPT_KEY STR(SOURCELUA_NAME) "_script"
#define SOURCELUA_SCHEDULER_KEY STR(SOURCELUA_NAME) "_scheduler"

#endif /* _luajit_hpp_ */