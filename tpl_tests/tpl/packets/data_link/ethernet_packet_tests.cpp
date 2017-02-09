//
// Created by ukheg on 26.09.2016.
//
#include <tpl/packets/data_link/ethernet_packet.hpp>
#include <gtest/gtest.h>

using namespace tpl;
using namespace utils;
using namespace packets;

namespace tpl_tests
{
    namespace packet{
        TEST(plain_ethernet_packet_tests, check_fields_layout){
            ASSERT_EQ(0, ethernet_packet::fields::destination_mac::position);
            ASSERT_EQ(6, ethernet_packet::fields::destination_mac::size);

            ASSERT_EQ(6, ethernet_packet::fields::source_mac::position);
            ASSERT_EQ(6, ethernet_packet::fields::source_mac::size);

            ASSERT_EQ(12, ethernet_packet::fields::payload_type::position);
            ASSERT_EQ(2, ethernet_packet::fields::payload_type::size);
        }

        TEST(plain_ethernet_packet_tests, can_get_header_and_payload_correct){
            ethernet_packet p(   { 0x00, 0x15, 0xf2, 0x78, 0x37, 0xf3, 0x00, 0x30, 0x48, 0x97, 0x12, 0x79, 0x08, 0x00,
                                   0x01, 0x02});
            ASSERT_EQ(byte_array({ 0x00, 0x15, 0xf2, 0x78, 0x37, 0xf3, 0x00, 0x30, 0x48, 0x97, 0x12, 0x79, 0x08, 0x00 }),
                      p.header_bytes());
            ASSERT_EQ(byte_array({ 0x01, 0x02 }), p.payload_bytes());
        }

        TEST(plain_ethernet_packet_tests, check_packet_fields_accessors)
        {
            ethernet_packet p({0x00, 0x15, 0xf2, 0x78, 0x37, 0xf3,
                               0x00, 0x30, 0x48, 0x97, 0x12, 0x79,
                               0x08, 0x00,
                               0x01, 0x02, 0x03});

            mac_address dest( byte_array{0x00, 0x15, 0xf2, 0x78, 0x37, 0xf3} );
            mac_address src( byte_array{0x00, 0x30, 0x48, 0x97, 0x12, 0x79} );

            ASSERT_EQ(dest, p.destination_mac());
            ASSERT_EQ(src, p.source_mac());
            ASSERT_EQ(ethernet_payload_types::ip, p.payload_type());
        }
    }
}

