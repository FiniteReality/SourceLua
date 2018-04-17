#include <stdexcept>

#include <source/tier1/tier1.hpp>
#include <source/tier2/tier2.hpp>

#include <common/luajit.hpp>
#include <common/logging.hpp>
#include <common/version.hpp>

#include <lua/libraries/event.hpp>
#include <lua/objects/common.hpp>
#include <lua/script.hpp>
#include <plugin/plugin.hpp>

namespace SourceLua
{

static Plugin* PLUGIN = nullptr;

Plugin* Plugin::GetActiveInstance()
{
    if (PLUGIN == nullptr)
    {
        throw std::runtime_error{"Plugin not initialized yet!"};
    }

    return PLUGIN;
}

bool Plugin::Load(CreateInterfaceFn interfaceFactory,
                  CreateInterfaceFn gameServerFactory)
{
    (void)gameServerFactory;
    ConnectTier1Libraries(&interfaceFactory, 1);
    ConnectTier2Libraries(&interfaceFactory, 1);

    _engine = static_cast<IVEngineServer*>(
                  interfaceFactory(INTERFACEVERSION_VENGINESERVER, nullptr)
              );
    _eventManager = static_cast<IGameEventManager2*>(
                        interfaceFactory(INTERFACEVERSION_GAMEEVENTSMANAGER2, nullptr)
                    );

    if (_engine == nullptr)
    {
        LogMessage<LogLevel::Error>("Failed to get instance of engine");
        return false;
    }

    if (_eventManager == nullptr)
    {
        LogMessage<LogLevel::Error>(
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
    Threading::Scheduler::Start(_G.get());

    ConVar_Register(0);

    LogMessage<LogLevel::Information> (
        "Version %s loaded successfully! Use 'sl_version' to see more.",
        SOURCELUA_VERSION);

    PLUGIN = this;
    return true;
}

void Plugin::Unload()
{
    Threading::Scheduler::Stop();

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
    _gameTickEvent.Fire([&](lua_State * L)
    {
        lua_pushboolean(L, simulating ? 1 : 0);
        return 1;
    });
}

void Plugin::Pause()
{
    _pauseEvent.Fire();
}
void Plugin::UnPause()
{
    _unPauseEvent.Fire();
}
void Plugin::LevelInit(char const* mapName)
{
    _levelChangingEvent.Fire([&](lua_State * L)
    {
        lua_pushstring(L, mapName);
        return 1;
    });
}
void Plugin::ServerActivate(edict_t* pEdictList, int edictCount,
                            int clientMax)
{
    _levelChangedEvent.Fire([&](lua_State * L)
    {
        lua_createtable(L, edictCount, 0);

        // TODO: is this safe? we could possibly read invalid memory this way
        int i;
        edict_t* edict;

        for (i = 0, edict = pEdictList; i < edictCount; i++, edict++)
        {
            Lua::Objects::ClassDefinition<edict_t>::PushValue(L, edict);
            lua_rawseti(L, -2, i + 1);
        }

        lua_pushinteger(L, clientMax);

        return 2;
    });
}
void Plugin::LevelShutdown() { }
PLUGIN_RESULT Plugin::ClientConnect(bool* allowConnect,
                                    edict_t* pEntity, const char* pName,
                                    const char* ipAddress, char* reject,
                                    int maxRejectLen)
{
    (void)allowConnect;
    (void)pEntity;
    (void)pName;
    (void)ipAddress;
    (void)reject;
    (void)maxRejectLen;
    return PLUGIN_CONTINUE;
}
void Plugin::ClientActive(edict_t* pEntity)
{
    (void)pEntity;
}
void Plugin::ClientDisconnect(edict_t* pEntity)
{
    (void)pEntity;
}
void Plugin::ClientPutInServer(edict_t* pEntity,
                               char const* playerName)
{
    (void)pEntity;
    (void)playerName;
}
void Plugin::ClientSettingsChanged(edict_t* pEntity)
{
    (void)pEntity;
}
PLUGIN_RESULT Plugin::ClientCommand(edict_t* pEntity,
                                    const CCommand& args)
{
    (void)pEntity;
    (void)args;
    return PLUGIN_CONTINUE;
}
PLUGIN_RESULT Plugin::NetworkIDValidated(const char* pName,
        const char* networkId)
{
    (void)pName;
    (void)networkId;
    return PLUGIN_CONTINUE;
}
void Plugin::OnQueryCvarValueFinished(QueryCvarCookie_t iCookie,
                                      edict_t* pEntity,
                                      EQueryCvarValueStatus eStatus,
                                      const char* name, const char* value)
{
    (void)iCookie;
    (void)pEntity;
    (void)eStatus;
    (void)name;
    (void)value;
}
void Plugin::OnEdictAllocated(edict_t* edict)
{
    (void)edict;
}
void Plugin::OnEdictFreed(const edict_t* edict)
{
    (void)edict;
}
void Plugin::FireGameEvent(IGameEvent* event)
{
    LogMessage<LogLevel::Debug>("Firing event %s", event->GetName());

    auto& luaEvent = Lua::Libraries::GetEvent(event->GetName());
    luaEvent.Fire([event](lua_State* L)
    {
        // HACK: we should deserialize to a table instead of passing the event
        Lua::Objects::ClassDefinition<IGameEvent>::PushValue(L, event);
        return 1;
    });
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
