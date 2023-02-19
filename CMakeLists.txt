if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(threadpoolcpp 
VERSION 1.0
LANGUAGES CXX)
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# take respectives flags for debug & release process 
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-std=c++11 -g -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-std=c++11 -g -O2")

# for clangd linting
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# default build type : Debug
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_subdirectory(Catch2)
add_subdirectory(tests)
