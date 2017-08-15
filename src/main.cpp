/*
 * Expose the plugin to Source
 */

#include <common/luajit.hpp>
#include <common/source.hpp>
#include <common/version.hpp>
#include <plugin/plugin.hpp>

using SourceLuaPlugin = SourceLua::Plugin;

SourceLuaPlugin plugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(SourceLuaPlugin, IServerPluginCallbacks,
    INTERFACEVERSION_ISERVERPLUGINCALLBACKS, plugin)

CON_COMMAND(sl_version, "Prints the currently running " SOURCELUA_NAME " Version")
{
    Msg(
        "%s Version %s by %s\nLuaJIT Version: %s %s\n",
        SOURCELUA_NAME, SOURCELUA_VERSION, SOURCELUA_AUTHOR,
        LUAJIT_VERSION, LUAJIT_COPYRIGHT);
}

CON_COMMAND(sl_eval, "Evaluate Lua code")
{
    if (args.ArgC() < 1)
    {
        Msg("Usage: sl_eval <lua code>\n");
        return;
    }
    
    const char* msg = plugin.RunLuaString(args.ArgS());

    if (msg != nullptr)
    {
        Warning("Lua error: %s\n", msg);
    }
}

static ConVar sl_version_major("sl_version_major", _STR(SOURCELUA_VERSION_MAJOR),
    FCVAR_NONE, SOURCELUA_NAME " Major Version");
static ConVar sl_version_minor("sl_version_minor", _STR(SOURCELUA_VERSION_MINOR),
    FCVAR_NONE, SOURCELUA_NAME " Minor Version");
static ConVar sl_version_patch("sl_version_patch", _STR(SOURCELUA_VERSION_PATCH),
    FCVAR_NONE, SOURCELUA_NAME " Patch Version");
static ConVar sl_version_full("sl_version_string", SOURCELUA_VERSION,
    FCVAR_NONE, SOURCELUA_NAME " Version");
static ConVar sl_version_luajit("sl_version_luajit", LUAJIT_VERSION,
    FCVAR_NONE, SOURCELUA_NAME " LuaJIT Version");