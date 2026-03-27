cmake_minimum_required(VERSION 3.25)
project(spbu-graph-dijkstra C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra")

add_library(graph graph.c)
add_library(queue queue.c)

target_link_libraries(graph PRIVATE queue)

add_executable(test_runner test.c)
target_link_libraries(test_runner PRIVATE graph)

add_executable(main main.c)
target_link_libraries(main PRIVATE graph)
