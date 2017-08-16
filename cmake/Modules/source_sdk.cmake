include(ExternalProject)

set(SOURCE_SDK_PLATFORM "mp")
set(SOURCE_SDK_BASE_DIR "${CMAKE_SOURCE_DIR}/source-sdk-2013/${SOURCE_SDK_PLATFORM}")

set(SOURCE_SDK_INCLUDE_DIRS
    "${SOURCE_SDK_BASE_DIR}/src"
    "${SOURCE_SDK_BASE_DIR}/src/common"
    "${SOURCE_SDK_BASE_DIR}/src/public"
    "${SOURCE_SDK_BASE_DIR}/src/public/tier0"
    "${SOURCE_SDK_BASE_DIR}/src/public/tier1"
)

if(WIN32)
    # TODO: we should accurately detect whether we're on xbox or windows
    add_definitions(-D_WIN32)
    set(SOURCE_SDK_LIBRARY_DIR "${SOURCE_SDK_BASE_DIR}/src/lib/public")
elseif(UNIX AND NOT APPLE)
    add_definitions(-DPOSIX -DLINUX -D_LINUX)
    set(SOURCE_SDK_LIBRARY_DIR "${SOURCE_SDK_BASE_DIR}/src/lib/public/linux32")
elseif(UNIX AND APPLE)
    add_definitions(-DPOSIX -DOSX)
    set(SOURCE_SDK_LIBRARY_DIR "${SOURCE_SDK_BASE_DIR}/src/lib/public/osx32")
endif()

if(MSVC)
    # TODO: we should probably set the version here too...
    add_definitions(-D_MSVC_VER)
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-DGNUC)
endif()