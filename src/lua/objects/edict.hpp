#ifndef _objedict_hpp_
#define _objedict_hpp_

#include <common/luajit.hpp>
#include <common/source.hpp>

namespace SourceLua
{
namespace Lua
{
namespace Objects
{

int WrapEdict(lua_State* L, edict_t* edict);

int luaopen_edict_wrapper(lua_State* L);

}
}
}

#endif /* _objedict_hpp_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
