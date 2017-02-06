cmake_minimum_required(VERSION 3.0)
project(tpl_lib)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(BOOST_ROOT C:/Projects/boost_1_61_0)
set(Googletest_DIR "${CMAKE_SOURCE_DIR}/3rd_party/googletest")

add_subdirectory(tpl)
add_subdirectory(tpl_tests)