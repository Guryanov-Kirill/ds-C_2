cmake_minimum_required(VERSION 3.25)

project(CSVProject C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra")

add_library(csv_logic CSV2.c)

add_executable(tests testCSV.c)
target_link_libraries(tests PRIVATE csv_logic)

add_executable(csv_app main.c)
target_link_libraries(csv_app PRIVATE csv_logic)
