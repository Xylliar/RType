cmake_minimum_required(VERSION 3.22)
project(RType LANGUAGES CXX)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(cmake/CPM.cmake)

CPMAddPackage(
    NAME freetype
    GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
    GIT_TAG VER-2-13-2
    GIT_SHALLOW TRUE
    EXCLUDE_FROM_ALL YES
    SYSTEM YES
    OPTIONS
        "FT_DISABLE_ZLIB ON"
        "FT_DISABLE_BZIP2 ON"
        "FT_DISABLE_PNG ON"
        "FT_DISABLE_HARFBUZZ ON"
        "FT_DISABLE_BROTLI ON"
)

set(SFML_USE_SYSTEM_DEPS OFF CACHE BOOL "" FORCE)
if (NOT WIN32)
    set(FREETYPE_LIBRARY ${freetype_BINARY_DIR}/libfreetype.a CACHE STRING "" FORCE)
endif()
set(FREETYPE_INCLUDE_DIRS ${freetype_SOURCE_DIR}/include CACHE STRING "" FORCE)

CPMAddPackage(
    NAME SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.2
    GIT_SHALLOW TRUE
    EXCLUDE_FROM_ALL YES
    SYSTEM YES
    OPTIONS "SFML_BUILD_AUDIO ON"
)

CPMAddPackage(
    NAME asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-18-1
    GIT_SHALLOW TRUE
    DOWNLOAD_ONLY YES
)
add_library(asio INTERFACE)
target_include_directories(asio SYSTEM INTERFACE ${asio_SOURCE_DIR}/asio/include)

add_subdirectory(utils)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(ecs)
add_subdirectory(poc/client)
add_subdirectory(poc/server)
