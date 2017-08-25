#ifndef _libluaevent_hpp_
#define _libluaevent_hpp_

/*
 * Event utilities for Lua
 */

#include <common/luajit.hpp>
#include <lua/event.hpp>

namespace SourceLua
{
namespace Lua
{
namespace Libraries
{

int luaopen_event(lua_State* L);
void register_event(const char* name, Event* event);

}
}
}

#endif /* _libluaevent_hpp_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
