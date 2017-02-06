cmake_minimum_required(VERSION 3.0)
project(tpl_tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_SYSTEM_NO_DEPRECATED")
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP  -D_WIN32_WINNT=0x0601  -D_SCL_SECURE_NO_WARNINGS")
elseif (GCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wa,-mbig-obj")
endif ()

##############################  Boost  #################################
find_package(Boost REQUIRED COMPONENTS system log log_setup filesystem locale)
if (NOT DEFINED Boost_FOUND)
    message(FATAL_ERROR "BOOST_ROOT not set")
endif ()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ALL_DYN_LINK -DBOOST_LOG_DYN_LINK")
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
set(AdditionalLibraries ${AdditionalLibraries} ${Boost_LIBRARIES} ws2_32)
##############################  Gtest(taken from googletest readme)  #################################
# Download and unpack googletest at configure time
configure_file("${CMAKE_SOURCE_DIR}/GtestCmakeLists.txt.in" googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()
########################################################################

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/tpl/include)
include_directories(${PROJECT_SOURCE_DIR})

file(GLOB TEST_FILES ${PROJECT_SOURCE_DIR}/test_files/*)
source_group("TestFiles" FILES ${TEST_FILES})

set(SOURCE_FILES
        main.cpp
        tpl/object_id_tests.cpp
        tpl/utils/d_array_test_class.cpp
        tpl/utils/data_size_test_class.cpp
        tpl/utils/ip_address_range_test_class.cpp
        tpl/utils/ip_direction_tests.cpp
        tpl/utils/ip_ranges_set_test_class.cpp
        )

add_executable(tpl_tests ${SOURCE_FILES} ${TEST_FILES})
target_link_libraries(tpl_tests tpl ${AdditionalLibraries})