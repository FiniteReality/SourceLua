#ifndef _plugin_hpp_
#define _plugin_hpp_

#include <memory>

#include <source/eiface.hpp>
#include <source/igameevents.hpp>
#include <source/tier1/interface.hpp>
#include <source/engine/iserverplugin.hpp>

#include <lua/event.hpp>
#include <thread/scheduler.hpp>

namespace SourceLua
{

class Plugin : public IServerPluginCallbacks, public IGameEventListener
{
    public:
        Plugin();
        Plugin(const Plugin&) = delete;
        Plugin& operator=(const Plugin&) = delete;
        Plugin(Plugin&&) = delete;
        Plugin& operator=(Plugin&&) = delete;
        ~Plugin() = default;

        static Plugin* GetActiveInstance();

        bool Load(CreateInterfaceFn interfaceFactory,
                  CreateInterfaceFn gameServerFactory) override;
        void Unload() override;

        void Pause() override;
        void UnPause() override;

        const char* GetPluginDescription() override;

        void LevelInit(char const* mapName) override;

        void ServerActivate(edict_t* pEdictList, int edictCount,
                            int clientMax) override;

        void GameFrame(bool simulating) override;

        void LevelShutdown() override;

        PLUGIN_RESULT ClientConnect(bool* allowConnect,
                                    edict_t* pEntity,
                                    const char* pName,
                                    const char* ipAddress,
                                    char* reject, int maxRejectLen) override;
        void ClientActive(edict_t* pEntity) override;
        void ClientDisconnect(edict_t* pEntity) override;
        void ClientPutInServer(edict_t* pEntity,
                               char const* playerName) override;
        void ClientSettingsChanged(edict_t* pEntity) override;
        PLUGIN_RESULT ClientCommand(edict_t* pEntity,
                                    const CCommand& args) override;

        PLUGIN_RESULT NetworkIDValidated(const char* pName,
                                         const char* networkId) override;

        void OnQueryCvarValueFinished(QueryCvarCookie_t iCookie,
                                      edict_t* pEntity,
                                      EQueryCvarValueStatus eStatus,
                                      const char* name,
                                      const char* value) override;

        void OnEdictAllocated(edict_t* edict) override;
        void OnEdictFreed(const edict_t* edict) override;

        void FireGameEvent(KeyValues* event) override;

        void SetCommandClient(int index) override;
        int GetCommandIndex();

        void RunLuaString(const char* code);
#ifdef SOURCELUA_DEBUG
        void CausePanic();
#endif /* SOURCELUA_DEBUG */

    private:
        void LoadLua();

        int _commandIndex;

        std::unique_ptr<lua_State, std::function<void(lua_State*)>> _G;

        Lua::Event& _gameTickEvent;
        Lua::Event& _levelChangingEvent;
        Lua::Event& _levelChangedEvent;
        Lua::Event& _pauseEvent;
        Lua::Event& _unPauseEvent;

        IVEngineServer* _engine;
        IGameEventManager2* _eventManager;
};

}

#endif /* _plugin_hpp_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
