cmake_minimum_required(VERSION 3.5)
project(converter)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O2 -pthread")

set(SOURCE_FILES ../src/main.cpp
                 ../lib/globals.h
                 ../lib/run.cpp
                 ../lib/run.hpp
                 ../lib/gcodeParser.cpp
                 ../lib/gcodeParser.hpp
                 ../lib/move.cpp
                 ../lib/move.hpp
        )

add_executable(piController ${SOURCE_FILES})
