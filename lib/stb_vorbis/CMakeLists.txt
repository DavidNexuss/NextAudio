cmake_minimum_required(VERSION 3.6)
project(stb_vorbis)
file(GLOB SRC *.cpp)
add_library(stb_vorbis ${SRC})
