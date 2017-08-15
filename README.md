# SourceLua: Integrating Lua with Source #

This is some temporary text until the repo is setup. Probably.

## Dependencies ##

- LuaJIT (included)
- Source SDK 2013 (Included)

## Build Instructions ##

1. Update submodules (`luajit-2.0`, `source-sdk-2013`)
2. Patch the Source SDK:
  `cd source-sdk-2013 && patch -t -Np1 < ../patches/source-sdk-2013.patch`
3. Make a build directory (e.g. `build`)
4. Run CMake (`cmake ..`)
5. Run Make (`make`)

## License ##

SourceLua is licensed under the MIT License.
See `LICENSE` for more information.