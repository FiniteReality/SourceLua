#ifndef _objcommon_hpp_
#define _objcommon_hpp_

// Creates a new instance of the given type, controlled by Lua
#define newUData(L, type) \
    static_cast<type*>(lua_newuserdata(L, sizeof(type)))

// Checks the Lua stack for the given userdata, and returns it.
#define checkUData(L, idx, name, type) \
    static_cast<type*>(luaL_checkudata(L, idx, name))

#endif /* _objcommon_hpp_ */
