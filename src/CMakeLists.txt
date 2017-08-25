include(luajit)
include(source_sdk)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${LUAJIT_INCLUDE_DIR})
include_directories(${TBB_INCLUDE_DIRS})
include_directories(${SOURCE_SDK_INCLUDE_DIRS})

set(SOURCE_FILES

lua/event.cpp lua/lua.cpp lua/scheduler.cpp

lua/libraries/console.cpp lua/libraries/event.cpp lua/libraries/thread.cpp

plugin/plugin.commands.cpp plugin/plugin.cvars.cpp plugin/plugin.events.cpp
plugin/plugin.cpp

main.cpp)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSOURCELUA_DEBUG")

add_library(sourcelua SHARED ${SOURCE_FILES})
add_dependencies(sourcelua luajit)
target_link_libraries(sourcelua ${LUAJIT_LIBRARIES}
"${SOURCE_SDK_LIBRARY_DIR}/libtier0.so" "${SOURCE_SDK_LIBRARY_DIR}/tier1.a"
"${SOURCE_SDK_LIBRARY_DIR}/tier2.a" tbb)

install(
    TARGETS sourcelua
    DESTINATION "${CMAKE_BINARY_DIR}/final"
)
