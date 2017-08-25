/*
 * Commands exposed by SourceLua
 */

#include <common/logging.hpp>
#include <plugin/plugin.hpp>

using namespace SourceLua;

CON_COMMAND(sl_version,
    "Prints the currently running " SOURCELUA_NAME " Version")
{
    (void)args;
    LogMessage<LogLevel::Information>(
        "Version %s by %s\nLuaJIT Version: %s %s\n",
        SOURCELUA_VERSION, SOURCELUA_AUTHOR,
        LUAJIT_VERSION, LUAJIT_COPYRIGHT);
}

CON_COMMAND(sl_eval, "Evaluate Lua code")
{
    auto plugin = Plugin::GetActiveInstance();

    if (args.ArgC() < 1)
    {
        Msg("Usage: sl_eval <lua code>\n");
        return;
    }

    const char* msg = plugin->RunLuaString(args.ArgS());

    if (msg != nullptr)
    {
        LogMessage<LogLevel::Warning>("Lua error: %s\n", msg);
    }
}

#ifdef SOURCELUA_DEBUG
CON_COMMAND_F(sl_force_panic, "Forces the Lua state to panic",
    FCVAR_SERVER_CAN_EXECUTE | FCVAR_HIDDEN)
{
    (void)args;
    auto plugin = Plugin::GetActiveInstance();
    plugin->CausePanic();
}
#endif /* SOURCELUA_DEBUG */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
