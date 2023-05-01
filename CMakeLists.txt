cmake_minimum_required(VERSION 3.9.4)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
  message(STATUS "Setting release build type")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
project(NextAudio CXX C)

add_subdirectory(lib/stb_vorbis)
add_subdirectory(lib/opus_decoder)
file(GLOB_RECURSE SRC src/*.cpp)

add_library(NextAudio ${SRC})
target_link_libraries(NextAudio PUBLIC stb_vorbis opus_decoder)
target_include_directories(NextAudio PUBLIC lib src)
