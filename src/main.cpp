/*
 * Expose the plugin to Source
 */

#include <common/logging.hpp>
#include <common/luajit.hpp>
#include <common/source.hpp>
#include <common/version.hpp>
#include <plugin/plugin.hpp>

using namespace SourceLua;
using SourceLuaPlugin = Plugin;

SourceLuaPlugin plugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(SourceLuaPlugin, IServerPluginCallbacks,
    INTERFACEVERSION_ISERVERPLUGINCALLBACKS, plugin)