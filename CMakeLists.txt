cmake_minimum_required(VERSION 3.5)

project(AdventOfCode2023)

include_directories(Source)

file(GLOB_RECURSE sourceFiles
    Source/*.cpp
    Source/*.h
)

add_executable(AdventOfCode2023 ${sourceFiles})
