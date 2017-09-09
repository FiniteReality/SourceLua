#ifndef _error_handler_hpp_
#define _error_handler_hpp_

#include <common/luajit.hpp>

namespace SourceLua
{
namespace Lua
{
namespace Errors
{

void HandleError(lua_State* L, int status);

}
}
}

#endif /* _error_handler_hpp_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
