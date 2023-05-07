cmake_minimum_required(VERSION 3.22.0)
project(LinkedList)

add_subdirectory(../Catch2 Catch2-build)
add_executable(LinkedListTest Test_main.cpp)
target_link_libraries(LinkedListTest PRIVATE Catch2::Catch2)

include(CTest)
include(Catch)