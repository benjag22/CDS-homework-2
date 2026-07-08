cmake_minimum_required(VERSION 3.16)
project(cds-hw2)

set(CMAKE_CXX_STANDARD 20)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif ()

add_executable(
    cds-hw2
    src/main.cpp
    src/AdjacencyList.cpp
    src/GraphImp.cpp
    include/k2tree/basic.c
    include/k2tree/bitrankw32int.c
    include/k2tree/buildtree.c
    include/k2tree/compressleaves.c
    include/k2tree/directcodes.c
    include/k2tree/hash.c
    include/k2tree/kTree.c
    include/k2tree/MemoryManager.c
    src/build_trees.hpp
)

target_include_directories(cds-hw2 PRIVATE include)
target_link_directories(cds-hw2 PRIVATE lib)
target_link_libraries(cds-hw2 sdsl divsufsort divsufsort64 m)
