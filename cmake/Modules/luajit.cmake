include(ExternalProject)

ExternalProject_Add(luajit
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/luajit-2.0"
    BINARY_DIR "${CMAKE_SOURCE_DIR}/luajit-2.0/src"
    GIT_SUBMODULES luajit-2.0
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND CFLAGS="${CMAKE_C_FLAGS}" LDFLAGS="${CMAKE_C_FLAGS}" make
    INSTALL_COMMAND ""
    TEST_COMMAND "")

set(LUAJIT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/luajit-2.0/src")
set(LUAJIT_LIBRARIES "${CMAKE_SOURCE_DIR}/luajit-2.0/src/libluajit.a")