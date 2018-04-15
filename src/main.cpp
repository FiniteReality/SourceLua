#include <source/tier1/interface.hpp>

#include <common/logging.hpp>
#include <common/version.hpp>

#include <plugin/plugin.hpp>

using namespace SourceLua;
using SourceLuaPlugin = Plugin;

SourceLuaPlugin plugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(SourceLuaPlugin, IServerPluginCallbacks,
    INTERFACEVERSION_ISERVERPLUGINCALLBACKS, plugin)
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
