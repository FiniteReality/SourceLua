#ifndef _luajit_hpp_
#define _luajit_hpp_

#include <lua.hpp>

#include <common/version.hpp>

#ifndef LUAJIT_VERSION
#error "LuaJIT wasn't included properly"
#endif

#define luaL_checklfunction(L, idx) \
    luaL_argcheck(L, lua_isfunction(L, idx) && \
                    !lua_iscfunction(L, idx), \
                    idx, "Lua function expected")

#define SOURCELUA_EVENT_KEY STR(SOURCELUA_NAME) "_event"
#define SOURCELUA_EVENT_CACHE_KEY STR(SOURCELUA_NAME) "_event_cache"
#define SOURCELUA_SCRIPT_KEY STR(SOURCELUA_NAME) "_script"
#define SOURCELUA_SCHEDULER_KEY STR(SOURCELUA_NAME) "_scheduler"

#endif /* _luajit_hpp_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
