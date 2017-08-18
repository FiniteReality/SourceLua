#ifndef _lua_hpp_
#define _lua_hpp_

/*
 * Lua abstractions
 */

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
        Script& operator=(Script&&) = delete;
        ~Script() = default;

        template <typename T>
        constexpr void PushInteger(T value)
        {
            static_assert(std::is_integral<T>::value, "T is not integral");
            lua_pushinteger(_T, (lua_Integer)value);
        }

        template <typename T>
        constexpr void PushFloat(T value)
        {
            static_assert(std::is_floating_point<T>::value, "T is not floating point");
            lua_pushnumber(_T, (lua_Number)value);
        }

        void PushString(const char* value);
        void PushString(const char* value, size_t length);

        void MoveFromParent(int amount);
        void MoveToParent(int amount);

        void Resume(uint64_t delta_millis);

        void Run(const char* code);
        void Run(const char* code, size_t length);

    private:
        lua_State* _L;
        lua_State* _T;
        const char* _name;
};

}
}

#endif /* _lua_hpp_ */