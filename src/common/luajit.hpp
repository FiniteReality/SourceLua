#ifndef _luajit_hpp_
#define _luajit_hpp_

/*
 * LuaJIT utilities
 */

#include <lua.hpp>

#include <common/version.hpp>

#ifndef LUAJIT_VERSION
#error "LuaJIT wasn't included properly, maybe we included a Lua version instead?"
#endif

#define SOURCELUA_PLUGIN_KEY STR(SOURCELUA_NAME) "_plugin"

#endif /* _luajit_hpp_ */