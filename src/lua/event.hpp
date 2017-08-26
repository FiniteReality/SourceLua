#ifndef _event_hpp_
#define _event_hpp_

#include <memory>
#include <vector>

#include <common/luajit.hpp>

namespace SourceLua
{
namespace Lua
{

class Event
{
    public:
        Event(lua_State* L);
        Event(lua_State* L, const char* _name);
        // Does not support copy or move operations
        Event(const Event&) = delete;
        Event& operator=(const Event&) = delete;
        Event(Event&&) = delete;
        Event& operator=(Event &&) = delete;

        void Fire(std::function<int(lua_State*)> pushArgs);

        void Connect(lua_State* L);

    private:

        lua_State* GetThread();

        struct Connection
        {
            int ref;
        };

        std::vector<Connection> connections;
        lua_State* L;
        const char* name;
};

}
}
#endif /* _event_hpp_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
