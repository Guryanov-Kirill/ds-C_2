cmake_minimum_required(VERSION 3.25)

project(aerosoft C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra")

add_library(avl_tree AVLTree.c)

add_executable(tests testAVLTree.c)
target_link_libraries(tests PRIVATE avl_tree)

add_executable(aerosoft_app main.c)
target_link_libraries(aerosoft_app PRIVATE avl_tree)

