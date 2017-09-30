#include <lua/objects/common.hpp>
#include <lua/objects/edict.hpp>

using namespace SourceLua::Lua;

struct EdictWrapper
{
    edict_t* edict;
};

void PushEdict(lua_State* L, edict_t* edict)
{
    EdictWrapper* wrapper = newUData(L, EdictWrapper);

    wrapper->edict = edict;
    luaL_getmetatable(L, SOURCELUA_EVENT_OBJECT_KEY);
    lua_setmetatable(L, -2);
}

int EdictEqual(lua_State* L)
{
    auto* lhs =
        checkUData(L, 1, SOURCELUA_EDICT_OBJECT_KEY, EdictWrapper);
    auto* rhs =
        checkUData(L, 2, SOURCELUA_EDICT_OBJECT_KEY, EdictWrapper);

    lua_pushboolean(L, lhs->edict == rhs->edict ? 1 : 0);
    return 1;
}

int EdictTostring(lua_State* L)
{
    auto* wrapper =
        checkUData(L, 1, SOURCELUA_EDICT_OBJECT_KEY, EdictWrapper);

    lua_pushstring(L, wrapper->edict->GetClassName());
    return 1;
}


int EdictFree(lua_State* L)
{
    auto* wrapper =
        checkUData(L, 1, SOURCELUA_EDICT_OBJECT_KEY, EdictWrapper);
    wrapper->edict->SetFree();
    return 0;
}
int EdictIsFree(lua_State* L)
{
    auto* wrapper =
        checkUData(L, 1, SOURCELUA_EDICT_OBJECT_KEY, EdictWrapper);

    lua_pushboolean(L, wrapper->edict->IsFree() ? 1 : 0);
    return 1;
}
int EdictGetFreeTime(lua_State* L)
{
    auto* wrapper =
        checkUData(L, 1, SOURCELUA_EDICT_OBJECT_KEY, EdictWrapper);

    lua_pushnumber(L, wrapper->edict->freetime);
    return 1;
}

int EdictChangeState(lua_State* L)
{
    auto* wrapper =
        checkUData(L, 1, SOURCELUA_EDICT_OBJECT_KEY, EdictWrapper);

    wrapper->edict->StateChanged();
    return 0;
}
int EdictIsStateChanged(lua_State* L)
{
    auto* wrapper =
        checkUData(L, 1, SOURCELUA_EDICT_OBJECT_KEY, EdictWrapper);

    lua_pushboolean(L, wrapper->edict->HasStateChanged() ? 1 : 0);
    return 1;
}

int EdictGetClassName(lua_State* L)
{
    auto* wrapper =
    checkUData(L, 1, SOURCELUA_EDICT_OBJECT_KEY, EdictWrapper);

    lua_pushstring(L, wrapper->edict->GetClassName());
    return 1;
}


static luaL_Reg edict_metamethods[] =
{
    {"__eq", EdictEqual},
    {"__tostring", EdictTostring},
    {nullptr, nullptr}
};

static luaL_Reg edict_methods[] =
{
    {"free", EdictFree},
    {"changeState", EdictChangeState},

    {"isFree", EdictIsFree},
    {"isStateChanged", EdictIsStateChanged},

    {"getFreeTime", EdictGetFreeTime},
    {"getClassName", EdictGetClassName},

    {nullptr, nullptr}
};


int Objects::WrapEdict(lua_State* L, edict_t* edict)
{
    PushEdict(L, edict);
    return 1;
}

int Objects::luaopen_edict_wrapper(lua_State* L)
{
    luaL_newmetatable(L, SOURCELUA_EDICT_OBJECT_KEY);
    luaL_register(L, nullptr, edict_metamethods);
    lua_pushliteral(L, "This metatable is locked");
    lua_setfield(L, -2, "__metatable");

    lua_newtable(L);
    luaL_register(L, nullptr, edict_methods);
    lua_setfield(L, -2, "__index");

    return 0;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
