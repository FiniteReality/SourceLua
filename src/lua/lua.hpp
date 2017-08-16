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
                constexpr void PushInteger(T value) const
                {
                    static_assert(std::is_integral<T>::value, "T is not integral");
                    lua_pushinteger(L, (lua_Integer)value);
                }

                template <typename T>
                constexpr void PushFloat(T value) const
                {
                    static_assert(std::is_floating_point<T>::value, "T is not floating point");
                    lua_pushnumber(L, (lua_Number)value);
                }

                void PushString(std::string value);
                void PushString(const std::string& value) const;
                void PushString(const char* value) const;
                void PushString(const char* value, size_t length) const;

                void MoveFromParent(int amount);
                void MoveToParent(int amount);

                void Resume(uint64_t delta_millis);

            private:
                lua_State* L;
                lua_State* T;
                const char* name;
        };
    }
}

#endif /* _lua_hpp_ */