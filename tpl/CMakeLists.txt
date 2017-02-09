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
        include/tpl/formats/pcap/detail/pcap_file_header.hpp
        include/tpl/formats/pcap/detail/pcap_record_header.h
        include/tpl/formats/pcap/pcap_file.hpp
        include/tpl/formats/pcap/pcap_istream.hpp
        include/tpl/formats/pcap/pcap_link_layer.h
        include/tpl/formats/pcap/pcap_ostream.hpp
        include/tpl/formats/pcap/pcap_record.hpp
        include/tpl/formats/pcap/pcap_records_container.hpp
        include/tpl/formats/pcap/pcap_version.hpp
        include/tpl/object_id.hpp
        include/tpl/packets/data_link/ethernet_packet.hpp
        include/tpl/packets/details/byte_sequence_field.hpp
        include/tpl/packets/details/enum_field.hpp
        include/tpl/packets/details/field_getter.hpp
        include/tpl/packets/details/field_setter.hpp
        include/tpl/packets/details/integer_field.hpp
        include/tpl/packets/details/packet_fields.hpp
        include/tpl/packets/field_descriptor.hpp
        include/tpl/packets/gre_packet.hpp
        include/tpl/packets/ip_packet.hpp
        include/tpl/packets/ip_v4_flags.hpp
        include/tpl/packets/ip_v4_packet.hpp
        include/tpl/packets/ip_v6_extension_header.hpp
        include/tpl/packets/ip_v6_extension_headers_sequence.hpp
        include/tpl/packets/ip_v6_packet.hpp
        include/tpl/packets/layouts/ethernet_packet_layout.hpp
        include/tpl/packets/layouts/gre_packet_layout.hpp
        include/tpl/packets/layouts/fields_layout.hpp
        include/tpl/packets/layouts/ip_packet_layout.hpp
        include/tpl/packets/layouts/ip_v4_layout.hpp
        include/tpl/packets/layouts/ip_v6_layout.hpp
        include/tpl/packets/layouts/transport_packet_layout.hpp
        include/tpl/packets/packet.hpp
        include/tpl/packets/packet_metainfo.hpp
        include/tpl/packets/packet_metainfo_pair.hpp
        include/tpl/packets/placeholders.hpp
        include/tpl/packets/transport/tcp_option.hpp
        include/tpl/packets/transport/tcp_options_sequence.hpp
        include/tpl/packets/transport/tcp_packet.hpp
        include/tpl/packets/transport/transport_packet.hpp
        include/tpl/packets/transport/transport_ports.hpp
        include/tpl/packets/transport/transport_type.hpp
        include/tpl/packets/transport/udp_packet.hpp
        include/tpl/tpl.hpp
        include/tpl/utils/bit_converter.hpp
        include/tpl/utils/d_array.hpp
        include/tpl/utils/data_size.hpp
        include/tpl/utils/ip_address_range.hpp
        include/tpl/utils/ip_direction.hpp
        include/tpl/utils/ip_range_set.hpp
        include/tpl/utils/mac_address.hpp
        include/tpl/utils/transport_conversation_id.hpp
        src/formats/pcap/detail/pcap_file_header.cpp
        src/formats/pcap/detail/pcap_record_header.cpp
        src/formats/pcap/pcap_file.cpp
        src/formats/pcap/pcap_istream.cpp
        src/formats/pcap/pcap_ostream.cpp
        src/formats/pcap/pcap_records_container.cpp
        src/object_id.cpp
        src/packets/data_link/ethernet_packet.cpp
        src/packets/gre_packet.cpp
        src/packets/ip_packet.cpp
        src/packets/ip_v4_packet.cpp
        src/packets/ip_v6_extension_header.cpp
        src/packets/ip_v6_extension_headers_sequence.cpp
        src/packets/ip_v6_packet.cpp
        src/packets/layouts/ethernet_packet_layout.cpp
        src/packets/layouts/fields_layout.cpp
        src/packets/layouts/ip_packet_layout.cpp
        src/packets/layouts/ip_v4_layout.cpp
        src/packets/layouts/ip_v6_layout.cpp
        src/packets/layouts/gre_packet_layout.cpp
        src/packets/layouts/transport_packet_layout.cpp
        src/packets/packet.cpp
        src/packets/packet_metainfo.cpp
        src/packets/transport/tcp_option.cpp
        src/packets/transport/tcp_options_sequence.cpp
        src/packets/transport/tcp_packet.cpp
        src/packets/transport/transport_packet.cpp
        src/packets/transport/transport_ports.cpp
        src/packets/transport/udp_packet.cpp
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