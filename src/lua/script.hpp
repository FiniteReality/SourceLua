#ifndef _lua_hpp_
#define _lua_hpp_

#include <cstdint>
#include <string>
#include <type_traits>

#include <common/luajit.hpp>

namespace SourceLua
{
namespace Lua
{

class Script
{
    public:
        Script(lua_State* L);
        Script(lua_State* L, const char* name);
        // Does not support copy or move operations
        Script(const Script&) = delete;
        Script& operator=(const Script&) = delete;
        Script(Script&&) = delete;
        Script& operator=(Script &&) = delete;
        ~Script();

        void Run(const char* code);
        void Run(const char* code, size_t length);

        const char* name() const;

    private:
        lua_State* _L;
        lua_State* _T;
        int thread_ref;
        std::string _name;
};

}
}

#endif /* _lua_hpp_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
