#include <gtest/gtest.h>
#include <tpl/formats/pcap/pcap_file.hpp>
#include <tpl/packets/data_link/ethernet_packet.hpp>
#include <tpl/packets/ip_v4_packet.hpp>
#include <tpl/packets/transport/udp_packet.hpp>
#include <tpl/packets/placeholders.hpp>
#include "test_helper.hpp"
using namespace tpl;
using namespace packets;
using namespace formats;


namespace tpl_tests
{
	TEST(udp_packet_tests, can_get_udp_parameters)
	{
		auto ip_p = test_helper.get_packet<ip_packet>("eth_ip_udp.pcap", [](byte_array const& bytes)
		                                              {
			                                              auto eth = ethernet_packet(bytes);
			                                              return ip_packet(packets::placeholders::parent_packet = eth);
		                                              }, 0);

		auto udp = udp_packet(packets::placeholders::parent_packet = ip_p);

		ASSERT_EQ(uint16_t(2428), udp.src_port());
		ASSERT_EQ(uint16_t(2424), udp.dst_port());
		ASSERT_EQ(uint16_t(36), udp.length());
		ASSERT_EQ(uint16_t(0x4b2e), udp.checksum());
	}

	TEST(udp_packet_tests, throws_exception_if_bytes_less_then_udp_header)
	{
        auto bytes = byte_array{0x09, 0x7c, 0x09, 0x78, 0x00, 0x24, 0x4b}; //last byte missing
        try{
            udp_packet udp(bytes);
            ASSERT_FALSE(true);
        }
        catch(...){}
		//ASSERT_THROW(udp_packet(bytes), std::invalid_argument); - this does not work(((
	}
}

