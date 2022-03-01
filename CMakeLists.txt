cmake_minimum_required(VERSION 3.15) # FetchContent is available in 3.11+
project(VirtualOscilloscope VERSION 0.1.0)

# specify the C/C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Adding Raylib
find_package(raylib 4.0 QUIET) # Let CMake search for a raylib-config.cmake
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
    FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG 4.0.0)
    FetchContent_MakeAvailable(raylib)
endif()

# This is the main part:

# Adding our source files
set(PROJECT_CODE "${CMAKE_CURRENT_LIST_DIR}/code")
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${PROJECT_CODE}/src/*.cpp") # Define PROJECT_SOURCES as a list of all source files
set(PROJECT_INCLUDE "${PROJECT_CODE}/include") # Define PROJECT_INCLUDE to be the path to the include directory of the project

# add the Config file
configure_file("${PROJECT_CODE}/Configs.hpp.in" "${PROJECT_INCLUDE}/Configs.hpp")

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)

# Setting ASSETS_PATH
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${PROJECT_CODE}/resources/") # Set the asset path macro to the absolute path on the dev machine
#target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="./assets") # Set the asset path macro in release mode to a relative path that assumes the assets folder is in the same directory as the game executable

if (WIN32)
    target_link_libraries(${PROJECT_NAME} WinMM)
endif()
# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
