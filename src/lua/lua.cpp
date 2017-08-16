/*
 * Lua abstractions
 */
#include <stdexcept>

#include <lua/lua.hpp>

namespace SourceLua
{
    namespace Lua
    {

        Script::Script(lua_State* _L)
            : L(_L), T(nullptr), name("Unknown")
        {
            if (L == nullptr)
                throw new std::runtime_error("L must not be null");

            T = lua_newthread(L);

            if (T == nullptr)
                throw new std::runtime_error("Could not initialize Lua thread");
        }

        Script::Script(lua_State* L, const char* _name)
            : Script(L)
        {
            name = _name;
        }

        void Script::PushString(std::string value)
        {
            lua_pushlstring(T, value.c_str(), value.length());
        }

        void Script::PushString(const std::string& value) const
        {
            lua_pushlstring(T, value.c_str(), value.length());
        }

        void Script::PushString(const char* value) const
        {
            lua_pushstring(T, value);
        }

        void Script::PushString(const char* value, size_t length) const
        {
            lua_pushlstring(T, value, length);
        }

        void Script::MoveFromParent(int amount)
        {
            lua_xmove(L, T, amount);
        }
        void Script::MoveToParent(int amount)
        {
            lua_xmove(T, L, amount);
        }

        void Script::Resume(uint64_t delta_millis)
        {
            lua_pushnumber(T, delta_millis / 1000);
            lua_resume(T, 1);
        }
    }
}