/*
 * Main plugin code
 */

#include <stdexcept>

#include <common/luajit.hpp>
#include <common/version.hpp>
#include <plugin/plugin.hpp>

#define PLUGIN_DESCRIPTION \
    "" SOURCELUA_NAME " " SOURCELUA_VERSION ", " SOURCELUA_AUTHOR

namespace SourceLua
{
    int SourceLua_AtPanic(lua_State *L)
    {
        const char* error = lua_tostring(L, lua_gettop(L));

        std::string message{"Lua panicked: "};
        message += error;
        throw new std::runtime_error(message);
    }

    bool Plugin::Load(CreateInterfaceFn interfaceFactory,
        CreateInterfaceFn gameServerFactory)
    {
        ConnectTier1Libraries(&interfaceFactory, 1);
        ConnectTier2Libraries(&interfaceFactory, 1);

        engine = (IVEngineServer*)interfaceFactory(INTERFACEVERSION_VENGINESERVER, NULL);
        eventManager = (IGameEventManager2*)interfaceFactory(INTERFACEVERSION_GAMEEVENTSMANAGER2, NULL);
        L = luaL_newstate();

        if (engine == nullptr)
        {
            DevMsg("Failed to get instance of engine");
            return false;
        }
        if (eventManager == nullptr)
        {
            DevMsg("Failed to get instance of event manager");
            return false;
        }
        if(L == nullptr)
        {
            DevMsg("Failed to init LuaJIT, possible memory allocation error?");
            return false;
        }

        lua_atpanic(L, SourceLua_AtPanic);
        luaL_openlibs(L);
        ConVar_Register(0);
        return true;
    }

    void Plugin::Unload()
    {
        if (L != nullptr)
        {
            lua_close(L);
        }

        ConVar_Unregister();
        DisconnectTier2Libraries();
        DisconnectTier1Libraries();
    }

    const char *Plugin::GetPluginDescription()
    {
        return PLUGIN_DESCRIPTION;
    }

    void Plugin::SetCommandClient(int index)
    {
        _commandIndex = index;
    }
    int Plugin::GetCommandIndex()
    {
        return _commandIndex;
    }

    const char* Plugin::RunLuaString(const char* code)
    {
        if (L == nullptr)
            return nullptr;

        if (luaL_dostring(L, code) != 0)
        {
            return lua_tostring(L, -1);
        }

        return nullptr;
    }

    void Plugin::Pause() { }
    void Plugin::UnPause() { }
    void Plugin::LevelInit(char const* mapName) { }
    void Plugin::ServerActivate(edict_t* pEdictList, int edictCount,
        int clientMax) { }
    void Plugin::GameFrame(bool simulating) { }
    void Plugin::LevelShutdown() { }
    PLUGIN_RESULT Plugin::ClientConnect(bool* allowConnect,
        edict_t* pEntity, const char* pName, const char* ipAddress,
        char* reject, int maxRejectLen)
    { return PLUGIN_CONTINUE; }
    void Plugin::ClientActive(edict_t* pEntity) { }
    void Plugin::ClientDisconnect(edict_t* pEntity) { }
    void Plugin::ClientPutInServer(edict_t* pEntity,
        char const* playerName) { }
    void Plugin::ClientSettingsChanged(edict_t* pEntity) { }
    PLUGIN_RESULT Plugin::ClientCommand(edict_t* pEntity,
        const CCommand& args)
    { return PLUGIN_CONTINUE; }
    PLUGIN_RESULT Plugin::NetworkIDValidated(const char* pName,
        const char* networkId)
    { return PLUGIN_CONTINUE; }
    void Plugin::OnQueryCvarValueFinished(QueryCvarCookie_t iCookie,
        edict_t* pEntity, EQueryCvarValueStatus eStatus,
        const char* name, const char* value) { }
    void Plugin::OnEdictAllocated(edict_t* edict) { }
    void Plugin::OnEdictFreed(const edict_t* edict) { }
    void Plugin::FireGameEvent(KeyValues* event) { }
}