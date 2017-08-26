#ifndef _lualibthread_hpp_
#define _lualibthread_hpp_

#include <common/luajit.hpp>

namespace SourceLua
{
namespace Lua
{
namespace Libraries
{

int luaopen_thread(lua_State* L);

}
}
}

#endif /* _lualibthread_hpp_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
