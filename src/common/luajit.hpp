#ifndef _luajit_hpp_
#define _luajit_hpp_

#include <lua.hpp>

#include <common/version.hpp>

#ifndef LUAJIT_VERSION
#error "LuaJIT wasn't included properly"
#endif

// Extra useful definitions which aren't included in Lua or LuaJIT
#define LUA_OK 0
#define luaL_checklfunction(L, idx) \
    luaL_argcheck(L, lua_isfunction(L, idx) && \
                  !lua_iscfunction(L, idx), \
                  idx, "Lua function expected")

// Table of functions used as callbacks to events
#define SOURCELUA_EVENT_CACHE_KEY \
    STR(SOURCELUA_NAME) "_event_cache"
// Dictionary of lua states to script objects
#define SOURCELUA_SCRIPT_CACHE_KEY \
    STR(SOURCELUA_NAME) "_script_cache"
// Table of lua states used in the thread scheduler
#define SOURCELUA_SCHEDULER_CACHE_KEY \
    STR(SOURCELUA_NAME) "_scheduler_cache"

#endif /* _luajit_hpp_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
