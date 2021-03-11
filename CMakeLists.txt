cmake_minimum_required (VERSION 3.12)
project (Engine)

set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
  message(STATUS "Build type not specified: Use Release by default")
endif(NOT CMAKE_BUILD_TYPE)

include_directories(./include/stb)

set(CMAKE_SUPPRESS_REGENERATION true)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

add_subdirectory(include)
add_subdirectory(src)

