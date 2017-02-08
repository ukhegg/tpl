cmake_minimum_required(VERSION 3.0)
project(tpl_lib)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(BOOST_ROOT C:/boost)
set(Googletest_DIR "${CMAKE_SOURCE_DIR}/3rd_party/googletest")

add_subdirectory(tpl)
add_subdirectory(tpl_tests)
add_subdirectory("${CMAKE_SOURCE_DIR}/3rd_party/googletest")

enable_testing()
add_test(TplTests tpl_tests)
