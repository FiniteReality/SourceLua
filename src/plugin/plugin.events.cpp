#include <stdexcept>

#include <common/luajit.hpp>
#include <common/logging.hpp>
#include <common/version.hpp>
#include <lua/lua.hpp>
#include <plugin/plugin.hpp>

namespace SourceLua
{
static Plugin* PLUGIN = nullptr;

Plugin* Plugin::GetActiveInstance()
{
    if (PLUGIN == nullptr)
    {
        throw new std::runtime_error("Plugin not initialized yet!");
    }

    return PLUGIN;
}

bool Plugin::Load(CreateInterfaceFn interfaceFactory,
                  CreateInterfaceFn gameServerFactory)
{
    ConnectTier1Libraries(&interfaceFactory, 1);
    ConnectTier2Libraries(&interfaceFactory, 1);

    _G = luaL_newstate();
    _scheduler = std::make_unique<Lua::Scheduler>();
    _engine = static_cast<IVEngineServer*>(
        interfaceFactory(INTERFACEVERSION_VENGINESERVER, nullptr)
    );
    _eventManager = static_cast<IGameEventManager2*>(
        interfaceFactory(INTERFACEVERSION_GAMEEVENTSMANAGER2, nullptr)
    );

    if (_engine == nullptr)
    {
        LogMessage<LogLevel::Error> ("Failed to get instance of engine");
        return false;
    }

    if (_eventManager == nullptr)
    {
        LogMessage<LogLevel::Error> (
            "Failed to get instance of event manager");
        return false;
    }

    if (_G == nullptr)
    {
        LogMessage<LogLevel::Error>(
            "Failed to init lua state, possible memory allocation failure?"
        );
        return false;
    }

    LoadLua();
    _scheduler->Start();

    ConVar_Register(0);

    LogMessage<LogLevel::Information> (
        "Version %s loaded successfully! Use 'sl_version' to see more.",
        SOURCELUA_VERSION);

    PLUGIN = this;
    return true;
}

void Plugin::Unload()
{
    _scheduler->Stop();
    lua_close(_G);

    ConVar_Unregister();
    DisconnectTier2Libraries();
    DisconnectTier1Libraries();
}

const char* Plugin::GetPluginDescription()
{
    return SOURCELUA_NAME " version " SOURCELUA_VERSION
           " by " SOURCELUA_AUTHOR;
}

void Plugin::SetCommandClient(int index)
{
    _commandIndex = index;
}

void Plugin::GameFrame(bool simulating)
{
    _scheduler->Tick();
}

void Plugin::Pause() { }
void Plugin::UnPause() { }
void Plugin::LevelInit(char const* mapName) { }
void Plugin::ServerActivate(edict_t* pEdictList, int edictCount,
                            int clientMax) { }
void Plugin::LevelShutdown() { }
PLUGIN_RESULT Plugin::ClientConnect(bool* allowConnect,
                                    edict_t* pEntity, const char* pName,
                                    const char* ipAddress, char* reject,
                                    int maxRejectLen)
{
    return PLUGIN_CONTINUE;
}
void Plugin::ClientActive(edict_t* pEntity) { }
void Plugin::ClientDisconnect(edict_t* pEntity) { }
void Plugin::ClientPutInServer(edict_t* pEntity,
                               char const* playerName) { }
void Plugin::ClientSettingsChanged(edict_t* pEntity) { }
PLUGIN_RESULT Plugin::ClientCommand(edict_t* pEntity,
                                    const CCommand& args)
{
    return PLUGIN_CONTINUE;
}
PLUGIN_RESULT Plugin::NetworkIDValidated(const char* pName,
        const char* networkId)
{
    return PLUGIN_CONTINUE;
}
void Plugin::OnQueryCvarValueFinished(QueryCvarCookie_t iCookie,
                                      edict_t* pEntity,
                                      EQueryCvarValueStatus eStatus,
                                      const char* name, const char* value) { }
void Plugin::OnEdictAllocated(edict_t* edict) { }
void Plugin::OnEdictFreed(const edict_t* edict) { }
void Plugin::FireGameEvent(KeyValues* event) { }
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
