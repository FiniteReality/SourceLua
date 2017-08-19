#ifndef _plugin_hpp_
#define _plugin_hpp_

/*
 * Main plugin definition
 */

#include <memory>

#include <common/source.hpp>
#include <thread/scheduler.hpp>

namespace SourceLua
{
class Plugin : public IServerPluginCallbacks, public IGameEventListener
{
    public:
        static Plugin* GetActiveInstance();

        bool Load(CreateInterfaceFn interfaceFactory,
            CreateInterfaceFn gameServerFactory) override;
        void Unload() override;

        void Pause() override;
        void UnPause() override;

        const char *GetPluginDescription() override;

        void LevelInit(char const* mapName) override;

        void ServerActivate(edict_t* pEdictList, int edictCount,
            int clientMax) override;

        void GameFrame(bool simulating) override;

        void LevelShutdown() override;

        PLUGIN_RESULT ClientConnect(bool* allowConnect,
            edict_t* pEntity, const char* pName, const char* ipAddress,
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
            edict_t* pEntity, EQueryCvarValueStatus eStatus,
            const char* name, const char* value) override;

        void OnEdictAllocated(edict_t* edict) override;
        void OnEdictFreed(const edict_t* edict) override;

        void FireGameEvent(KeyValues* event) override;

        void SetCommandClient(int index) override;
        int GetCommandIndex();

        const char* RunLuaString(const char* code);
        #ifdef SOURCELUA_DEBUG
        void CausePanic();
        #endif /* SOURCELUA_DEBUG */

    private:
        int _commandIndex;

        lua_State* _G;
        std::unique_ptr<Scheduling::Scheduler> _scheduler;

        IVEngineServer* _engine;
        IGameEventManager2* _eventManager;
};
}

#endif /* _plugin_hpp_ */