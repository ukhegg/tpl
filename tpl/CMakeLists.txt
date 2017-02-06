cmake_minimum_required(VERSION 3.6)
project(tpl)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_SYSTEM_NO_DEPRECATED -DTPL_DLL_EXPORTS")

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP -D_WIN32_WINNT=0x0601 /wd\"4251\" /wd\"4275\" /wd\"4503\" -D_SCL_SECURE_NO_WARNINGS")
elseif (MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -DASSIMP_BUILD_ASSIMP_TOOLS=NO -DASSIMP_BUILD_TESTS=OFF")
endif ()

if (BUILD_SHARED_LIBS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTPL_DLL")
endif ()

#################################  Boost ######################################
find_package(Boost REQUIRED COMPONENTS system log filesystem locale)
if (NOT DEFINED Boost_FOUND)
    message(FATAL_ERROR "BOOST_ROOT not set")
endif ()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ALL_DYN_LINK -DBOOST_LOG_DYN_LINK=1")
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
set(AdditionalLibraries ${AdditionalLibraries} ${Boost_LIBRARIES} ws2_32)
###############################################################################
include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCE_FILES
        include/tpl/object_id.hpp
        include/tpl/tpl.hpp
        include/tpl/utils/d_array.hpp
        include/tpl/utils/data_size.hpp
        include/tpl/utils/ip_address_range.hpp
        include/tpl/utils/ip_direction.hpp
        include/tpl/utils/ip_range_set.hpp
        include/tpl/utils/mac_address.hpp
        include/tpl/utils/transport_conversation_id.hpp
        src/utils/data_size.cpp
        src/utils/ip_address_range.cpp
        src/utils/ip_direction.cpp
        src/utils/ip_range_set.cpp
        src/utils/mac_address.cpp
        )


add_library(tpl ${SOURCE_FILES})
target_link_libraries(tpl ${AdditionalLibraries} )

install(TARGETS tpl
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)