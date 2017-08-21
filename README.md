# SourceLua: Integrating Lua with Source #

This is some temporary text until the repo is setup. Probably.

## This project is still a work in progress. ##

Do not expect to be able to use it to write your next server plugin right
now! SourceLua does nothing useful outside of debug mode, so you can't even
write Lua code to interact with anything.

We are looking for contributors to help get this project off the ground,
so any help would be great.

## Dependencies ##

- A C++ compiler supporting C++14
- CMake
- LuaJIT (included)
- Source SDK 2013 (Included)
- TBB (Not included)

## Build Instructions ##

### Linux ###
These instructions were tested on a Debian 9 Stretch install, they may
require tweaking to run correctly on your system.
1. Update submodules (`luajit-2.0`, `source-sdk-2013`)
2. Patch the Source SDK:
  `cd source-sdk-2013 && git apply ../patches/source-sdk-2013.patch`
3. Make a build directory (e.g. `build`)
4. Run CMake (`cmake ..`)
5. Run Make (`make`)
6. Build outputs are located in `final`

## Installation Instructions ##

1. Build SourceLua if it isn't already built
2. Create a folder in your server's game folder (e.g. for Counter-Strike:
   Source, it would be `$INSTALL_DIR/cstrike/` where `$INSTALL_DIR` is where
   the server is installed) named `addons`, if it doesn't already exist
3. Move `util/sourcelua.vdf` into the `addons` folder you created
3. Create a folder named `sourcelua` in the same directory you moved
   `sourcelua.vdf` into, and a folder in that named `bin`
4. Move the contents of `final` into the `bin` folder you created
5. Restart the server if it was running already

## License ##

SourceLua is licensed under the MIT License. See `LICENSE` for more
information. Additionally, read `source-sdk-2013/LICENSE` and
`source-sdk-2013/thirdpartylegalnotices.txt` for legal information related to
the use of the Source SDK.
