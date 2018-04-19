#ifndef _libluaevent_hpp_
#define _libluaevent_hpp_

#include <source/igameevents.hpp>

#include <common/luajit.hpp>
#include <lua/event.hpp>

namespace SourceLua
{
namespace Lua
{
namespace Libraries
{

int luaopen_event(lua_State* L);
Event& GetEvent(const std::string name);
void PushGameEvent(lua_State* L,
    IGameEvent* event);

}
}
}

#endif /* _libluaevent_hpp_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
