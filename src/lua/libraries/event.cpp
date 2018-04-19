#include <stdexcept>
#include <tbb/concurrent_unordered_map.h>
#include <unordered_map>

#include <lua/objects/common.hpp>
#include <lua/libraries/event.hpp>

#include <plugin/plugin.hpp>

using namespace SourceLua::Lua;

enum class FieldType {
    String,
    Boolean,
    Byte,
    Short,
    Long,
    Float
};
const char* const FieldTypeStrings[]{
    "string",
    "boolean",
    "byte",
    "short",
    "long",
    "float"
};

using LuaEvent = Objects::ClassDefinition<Event>;
using EventRegistry = tbb::concurrent_unordered_map<std::string, Event>;
using DescRegistry = tbb::concurrent_unordered_map<std::string,
    std::unordered_map<std::string, FieldType>>;


static EventRegistry EventMap;
static DescRegistry SourceEventDescriptors;

int GetEvent(lua_State* L)
{
    const char* name = luaL_checkstring(L, 1);

    Event& event = Libraries::GetEvent(name);

    LuaEvent::PushValue(L, &event);

    return 1;
}

int DescribeEvent(lua_State* L)
{
    const char* name = luaL_checkstring(L, 1);
    int nargs = lua_gettop(L);

    /* N.B. Multiple plugins might try to describe the same event, so we
     * bail-out if we're already described
     */
    auto iter = SourceEventDescriptors.find(std::string{name});
    if (iter != SourceEventDescriptors.end())
        return 0;

    #define tablecheck(L, stackidx, tabidx, expected) \
        { \
            lua_rawgeti(L, stackidx, tabidx); \
            int type = lua_type(L, -1); \
            if (type != (expected)) \
                return luaL_error(L, \
                    "bad table %d, index %d: expected %s, got %s", \
                    stackidx, tabidx, \
                    lua_typename(L, expected), lua_typename(L, type) \
                ); \
        }

    std::unordered_map<std::string, FieldType> description;

    for (int i = 2; i <= nargs; i++)
    {
        luaL_checktype(L, i, LUA_TTABLE);

        tablecheck(L, i, 1, LUA_TSTRING);
        tablecheck(L, i, 2, LUA_TSTRING);

        const char* fieldName = lua_tostring(L, -2);
        FieldType fieldType = static_cast<FieldType>(
            luaL_checkoption(L, -1, nullptr, FieldTypeStrings));

        description.emplace(fieldName, fieldType);
    }

    #undef tablecheck

    auto* plugin = SourceLua::Plugin::GetActiveInstance();
    auto* eventManager = plugin->GetEventManager();

    if (!eventManager->FindListener(plugin, name))
        eventManager->AddListener(plugin, name, true);

    SourceEventDescriptors.emplace(std::string{name}, std::move(description));

    Event& event = Libraries::GetEvent(name);
    LuaEvent::PushValue(L, &event);

    return 1;
}

static luaL_Reg event_funcs[] =
{
    {"get", GetEvent},
    {"describe", DescribeEvent},
    {nullptr, nullptr}
};


int Libraries::luaopen_event(lua_State* L)
{
    luaL_register(L, "event", event_funcs);
    return 1;
}

Event& Libraries::GetEvent(const std::string name)
{
    EventRegistry::const_iterator iter = EventMap.find(name);

    if (iter != EventMap.end())
    {
        return iter->second;
    }
    else
    {
        bool success;
        std::tie(iter, success) = EventMap.emplace(name, Event{name});

        if (success)
            return iter->second;
        else
            throw std::runtime_error{"Failed to construct event type"};
    }
}

void Libraries::PushGameEvent(lua_State* L,
    IGameEvent* event)
{
    std::string eventName{event->GetName()};
    DescRegistry::const_iterator iter = SourceEventDescriptors.find(eventName);

    if (iter == SourceEventDescriptors.end())
    {
        throw std::runtime_error{"Descriptor not registered for event"};
    }

    auto descriptor = iter->second;
    lua_createtable(L, 0, descriptor.size());

    for (auto& pair : descriptor)
    {
        const char* name = pair.first.c_str();
        lua_pushstring(L, name);
        switch (pair.second)
        {
            case FieldType::String:
                lua_pushstring(L, event->GetString(name));
                break;
            case FieldType::Boolean:
                lua_pushboolean(L, event->GetBool(name) ? 1 : 0);
                break;
            case FieldType::Byte:
            case FieldType::Short:
            case FieldType::Long:
                lua_pushnumber(L, event->GetInt(name));
                break;
            case FieldType::Float:
                lua_pushnumber(L, event->GetFloat(name));
                break;
        }

        lua_rawset(L, -3);
    }
}


// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
