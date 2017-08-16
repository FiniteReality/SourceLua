/*
 * Main plugin code
 */

#include <stdexcept>

#include <common/luajit.hpp>
#include <common/logging.hpp>
#include <common/version.hpp>
#include <plugin/plugin.hpp>

#define PLUGIN_DESCRIPTION \
    "" SOURCELUA_NAME " " SOURCELUA_VERSION ", " SOURCELUA_AUTHOR

namespace SourceLua
{
    Plugin::Plugin()
    {
        if(static_instance != nullptr)
            throw new std::runtime_error("Only one instance of "
                SOURCELUA_NAME "::Plugin may be active at a time");

        static_instance = this;
    }

    Plugin* Plugin::GetActiveInstance()
    {
        return static_instance;
    }

    bool Plugin::Load(CreateInterfaceFn interfaceFactory,
        CreateInterfaceFn gameServerFactory)
    {
        ConnectTier1Libraries(&interfaceFactory, 1);
        ConnectTier2Libraries(&interfaceFactory, 1);

        _engine = static_cast<IVEngineServer*>(interfaceFactory(INTERFACEVERSION_VENGINESERVER, nullptr));
        _eventManager = static_cast<IGameEventManager2*>(interfaceFactory(INTERFACEVERSION_GAMEEVENTSMANAGER2, nullptr));

        if (_engine == nullptr)
        {
            LogMessage<LogLevel::Error>("Failed to get instance of engine");
            return false;
        }
        if (_eventManager == nullptr)
        {
            LogMessage<LogLevel::Error>("Failed to get instance of event manager");
            return false;
        }

        ConVar_Register(0);
        return true;
    }

    void Plugin::Unload()
    {
        ConVar_Unregister();
        DisconnectTier2Libraries();
        DisconnectTier1Libraries();
    }

    const char* Plugin::GetPluginDescription()
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
        /*if (L == nullptr)
            return nullptr;

        if (luaL_dostring(L, code) != 0)
        {
            return lua_tostring(L, -1);
        }*/

        return nullptr;
    }

#ifdef SOURCELUA_DEBUG
    void Plugin::CausePanic()
    {
        /*lua_pushnil(L);
        lua_call(L, 0, 0);*/
    }
#endif /* SOURCELUA_DEBUG */

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