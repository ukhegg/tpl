cmake_minimum_required(VERSION 3.0)
project(tpl_tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_SYSTEM_NO_DEPRECATED")
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP  -D_WIN32_WINNT=0x0601  -D_SCL_SECURE_NO_WARNINGS")
elseif (GCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wa,-mbig-obj")
endif ()

set(AdditionalLibraries tpl)
##############################  Boost  #################################
find_package(Boost REQUIRED COMPONENTS system log filesystem locale)
if (NOT DEFINED Boost_FOUND)
    message(FATAL_ERROR "BOOST_ROOT not set")
endif ()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ALL_DYN_LINK -DBOOST_LOG_DYN_LINK")
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
set(AdditionalLibraries ${AdditionalLibraries} ${Boost_LIBRARIES})
if(WIN32)
    set(AdditionalLibraries ${AdditionalLibraries} ws2_32)
endif(WIN32)
##############################  Gtest  #################################
include_directories("${gtest_SOURCE_DIR}/include")
set(AdditionalLibraries ${AdditionalLibraries} gtest)
########################################################################

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/tpl/include)
include_directories(${PROJECT_SOURCE_DIR})

file(GLOB TEST_FILES ${PROJECT_SOURCE_DIR}/test_files/*)
source_group("TestFiles" FILES ${TEST_FILES})

set(SOURCE_FILES
        main.cpp
        test_helper.cpp
        test_helper.hpp
        tpl/formats/pcap_file_test_class.cpp
        tpl/object_id_tests.cpp
        tpl/packets/data_link/ethernet_packet_tests.cpp
        tpl/packets/transport/tcp_packet_test_class.cpp
        tpl/packets/transport/transport_type_tests.cpp
        tpl/packets/transport/udp_packet_test_class.cpp
        tpl/packets/field_descryptor_test_class.cpp
        tpl/packets/gre_packet_test_class.cpp
        tpl/packets/ip_packet_test_class.cpp
        tpl/packets/packet_id_tests.cpp
        tpl/packets/packet_tests.cpp
        tpl/utils/d_array_test_class.cpp
        tpl/utils/data_size_test_class.cpp
        tpl/utils/field_descriptor_tests.cpp
        tpl/utils/ip_address_range_test_class.cpp
        tpl/utils/ip_direction_tests.cpp
        tpl/utils/ip_ranges_set_test_class.cpp
        tpl/utils/packet_metainfo_test_class.cpp
        )

add_executable(tpl_tests ${SOURCE_FILES} ${TEST_FILES})
target_link_libraries(tpl_tests ${AdditionalLibraries})