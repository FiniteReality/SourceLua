#ifndef _lualibconsole_hpp_
#define _lualibconsole_hpp_

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
