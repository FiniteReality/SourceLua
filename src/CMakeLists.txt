include(luajit)
include(source_sdk)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${LUAJIT_INCLUDE_DIR})
include_directories(${SOURCE_SDK_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp plugin/plugin.cpp plugin/cvars.cpp
plugin/commands.cpp lua/lua.cpp thread/scheduler.cpp)

set(CMAKE_CXX_FLAGS_DEBUG -DSOURCELUA_DEBUG)

add_library(sourcelua SHARED ${SOURCE_FILES})
add_dependencies(sourcelua luajit)
target_link_libraries(sourcelua ${LUAJIT_LIBRARIES}
"${SOURCE_SDK_LIBRARY_DIR}/libtier0.so" "${SOURCE_SDK_LIBRARY_DIR}/tier1.a"
"${SOURCE_SDK_LIBRARY_DIR}/tier2.a")