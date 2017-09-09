#ifndef _event_hpp_
#define _event_hpp_

#include <functional>
#include <memory>
#include <unordered_set>

#include <common/luajit.hpp>

namespace SourceLua
{
namespace Lua
{

class Event
{
    public:
        Event(lua_State* L, const std::string name);
        // Does not support copy or move operations
        Event(const Event&) = delete;
        Event& operator=(const Event&) = delete;
        Event(Event&&) = delete;
        Event& operator=(Event &&) = delete;
        ~Event();

        void Fire(std::function<int(lua_State*)> pushArgs = nullptr);

        int Connect(lua_State* L);
        bool Disconnect(int ref);

        const std::string name;

    private:

        lua_State* GetThread();

        std::unordered_set<int> _connections;
        lua_State* _L;
};

}
}
#endif /* _event_hpp_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
