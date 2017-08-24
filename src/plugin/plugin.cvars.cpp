/*
 * CVars exposed by SourceLua.
 */

#include <plugin/plugin.hpp>

// TODO: decide how to expose these to parts of the plugin

static ConVar sl_version_major("sl_version_major",
                               STR(SOURCELUA_VERSION_MAJOR), FCVAR_NONE,
                               SOURCELUA_NAME " Major Version");
static ConVar sl_version_minor("sl_version_minor",
                               STR(SOURCELUA_VERSION_MINOR), FCVAR_NONE,
                               SOURCELUA_NAME " Minor Version");
static ConVar sl_version_patch("sl_version_patch",
                               STR(SOURCELUA_VERSION_PATCH), FCVAR_NONE,
                               SOURCELUA_NAME " Patch Version");
static ConVar sl_version_full("sl_version_string", SOURCELUA_VERSION,
                              FCVAR_NONE, SOURCELUA_NAME " Version");
static ConVar sl_version_luajit("sl_version_luajit", LUAJIT_VERSION,
                                FCVAR_NONE, SOURCELUA_NAME " LuaJIT Version");
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
