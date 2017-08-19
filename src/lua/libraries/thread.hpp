#ifndef _libthread_hpp_
#define _libthread_hpp_

#include <common/luajit.hpp>

#include <thread/scheduler.hpp>

namespace SourceLua
{
namespace Lua
{
namespace Threading
{
    int luaopen_thread(lua_State* L);
}
}
}

#endif /* _libthread_hpp_ */