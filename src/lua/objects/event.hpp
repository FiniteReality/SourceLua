#ifndef _objevent_hpp_
#define _objevent_hpp_

#include <common/luajit.hpp>

namespace SourceLua
{
namespace Lua
{
namespace Objects
{

int WrapEvent(lua_State* L, Event* event);

int luaopen_event_wrapper(lua_State* L);

}
}
}

#endif /* _objevent_hpp_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
