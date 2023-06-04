cmake_minimum_required(VERSION 3.6)
project(opus_decoder)
file(GLOB SRC *.cpp)
add_library(opus_decoder ${SRC})
