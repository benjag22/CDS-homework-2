cmake_minimum_required(VERSION 3.16)
project(cds-hw2)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    cds-hw2
    src/main.cpp
    src/AdjacencyList.cpp
    src/GraphImp.cpp
)

target_include_directories(cds-hw2 PRIVATE include)
target_link_directories(cds-hw2 PRIVATE lib)
target_link_libraries(cds-hw2 sdsl divsufsort divsufsort64 m)
