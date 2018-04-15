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
        Event(const std::string name);

        Event(const Event&) = delete;
        Event& operator=(const Event&) = delete;

        Event(Event&&) = default;
        Event& operator=(Event &&) = default;

        ~Event() = default;

        void Fire(std::function<int(lua_State*)> pushArgs = nullptr);

        int Connect(lua_State* L);
        bool Disconnect(lua_State* L, int ref);

        inline const std::string name() const { return _name; };

    private:
        std::unordered_set<int> _connections;
        std::string _name;
};

}
}
#endif /* _event_hpp_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
