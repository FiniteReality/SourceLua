#ifndef _lualibconsole_hpp_
#define _lualibconsole_hpp_

/*
 * Console utilities for Lua
 */

#include <common/luajit.hpp>

namespace SourceLua
{
namespace Lua
{
namespace Libraries
{

int luaopen_console(lua_State* L);

}
}
}

#endif /* _lualibconsole_hpp_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
