#include <gtest/gtest.h>
#include <tpl/formats/pcap/pcap_file.hpp>
#include <tpl/packets/ip_v4_packet.hpp>
#include <tpl/packets/ip_v6_packet.hpp>
#include <tpl/packets/data_link/ethernet_packet.hpp>
#include "test_helper.hpp"
using namespace tpl;
using namespace tpl::packets;
using namespace tpl::formats;

namespace tpl_tests
{
	TEST(ip_packet_tests, ip_v4_packet_gets_right_base_parameters)
	{
		auto ipv4_packet = test_helper.get_packet<ip_v4_packet>("eth_ip_https.pcap",
		                                                        [](const byte_array& b)
		                                                        {
			                                                        return ip_v4_packet(b.suffix(ethernet_packet::fields::header_length));
		                                                        },
		                                                        1);

		ASSERT_EQ(ip_version::ip_v4, ipv4_packet.version());

		ASSERT_EQ(uint8_t(24), ipv4_packet.header_length());
		ASSERT_EQ(uint8_t(0), ipv4_packet.differentiated_services());
		ASSERT_EQ(uint16_t(32), ipv4_packet.total_length());
		ASSERT_EQ(uint16_t(0x1367), ipv4_packet.identification());
		ASSERT_FALSE(ipv4_packet.flags().reserved_bit);
		ASSERT_FALSE(ipv4_packet.flags().do_not_fragment);
		ASSERT_FALSE(ipv4_packet.flags().more_fragments);
		ASSERT_EQ(uint16_t(0), ipv4_packet.fragment_offset());
		ASSERT_EQ(uint8_t(1), ipv4_packet.ttl());
		ASSERT_EQ(ip_payload_protocols::igmp, ipv4_packet.next_header());
		ASSERT_EQ(uint16_t(0x0000), ipv4_packet.header_checksum());
		ASSERT_EQ(boost::asio::ip::address_v4::from_string("169.254.255.66"), ipv4_packet.source());
		ASSERT_EQ(boost::asio::ip::address_v4::from_string("224.0.0.2"), ipv4_packet.destination());
	}

	TEST(ip_packet_tests, ip_v4_packet_options_length)
	{
		auto ipv4_packet = test_helper.get_packet<ip_v4_packet>("eth_ip_https.pcap",
		                                                        [](const byte_array& b)
		                                                        {
			                                                        return ip_v4_packet(b.suffix(ethernet_packet::fields::header_length));
		                                                        },
		                                                        1);
		ASSERT_EQ(size_t(4), ipv4_packet.options_bytes().size());
		ASSERT_EQ(uint8_t(0x94), ipv4_packet.options_bytes()[0]);
		ASSERT_EQ(uint8_t(0x00), ipv4_packet.options_bytes()[3]);
	}

	std::vector<ip_v6_extension_header> get_ext_headers(ip_v6_packet const& p)
    {
        return std::vector<ip_v6_extension_header>(p.extension_headers().begin(), p.extension_headers().end());
    }

	TEST(ip_packet_tests, ip_v6_packet_get_rigth_base_parameters)
	{
		auto ipv6_packet = test_helper.get_packet<ip_v6_packet>("eth_ip_https.pcap",
		                                                        [](const byte_array& b)
		                                                        {
			                                                        return ip_v6_packet(b.suffix(ethernet_packet::fields::header_length));
		                                                        },
		                                                        4);

		ASSERT_EQ(ip_version::ip_v6, ipv6_packet.version());
		ASSERT_EQ(uint8_t(0), ipv6_packet.traffic_class());
		ASSERT_EQ(uint32_t(0), ipv6_packet.flow_label());
		ASSERT_EQ(uint16_t(41), ipv6_packet.payload_length());
		ASSERT_EQ(ip_payload_protocols::udp, ipv6_packet.next_header());
		ASSERT_EQ(uint8_t(1), ipv6_packet.hop_limit());

        auto ext_headers = get_ext_headers(ipv6_packet);
		ASSERT_EQ(size_t(0), ext_headers.size());
		ASSERT_EQ(size_t(40), ipv6_packet.header_bytes().size());
		ASSERT_EQ(size_t(41), ipv6_packet.payload_bytes().size());
	}

	TEST(ip_packet_tests, ip_v6_packet_can_parse_extention_headers)
	{
		auto ipv6_packet = test_helper.get_packet<ip_v6_packet>("eth_ip_https.pcap",
		                                                        [](const byte_array& b)
		                                                        {
			                                                        return ip_v6_packet(b.suffix(ethernet_packet::fields::header_length));
		                                                        },
		                                                        7);

		ASSERT_EQ(ip_version::ip_v6, ipv6_packet.version());
		ASSERT_EQ(uint8_t(0), ipv6_packet.traffic_class());
		ASSERT_EQ(uint32_t(0), ipv6_packet.flow_label());
		ASSERT_EQ(uint16_t(36), ipv6_packet.payload_length());
		ASSERT_EQ(ip_payload_protocols::hopopts, ipv6_packet.next_header());
		ASSERT_EQ(uint8_t(1), ipv6_packet.hop_limit());

		ASSERT_EQ(size_t(40), ipv6_packet.header_bytes().size());
		ASSERT_EQ(size_t(36), ipv6_packet.payload_bytes().size());

        auto ext_headers = get_ext_headers(ipv6_packet);
        ASSERT_EQ(size_t(1), ext_headers.size());
		ip_v6_extension_header hop_option = ext_headers[0];
		ASSERT_EQ(ip_payload_protocols::icmpv6, hop_option.next_header());
		ASSERT_EQ(uint16_t(8), hop_option.total_length());
	}

	TEST(ip_packet_tests, returns_nullptr_ip_v4_packet_if_bytes_less_than_in_header_length)
	{
		auto bytes = byte_array({46, 0x00, 0xce, 0x97,
			                        0x24, 0xdf, 0xdd, 0x9c,
			                        0x00, 0x00, 0x00, 0x00,
			                        0x00, 0x00, 0x00, 0x00,
			                        0x00, 0x00, 0x00});//46 stands for 24 bytes in header and we've got 23
		auto ip_v4 = ip_v4_packet(bytes);
		//ASSERT_TRUE(ip_v4 == nullptr);
	}

	TEST(ip_packet_tests, returns_nullptr_ip_v4_packet_if_less_bytes_than_min_possible)
	{
		auto bytes = byte_array({40, 0x00, 0xce, 0x97, 0x24, 0xdf, 0xdd, 0x9c});//it must be at least 20 bytes
		auto ip_v4 = ip_v4_packet(bytes);
		//ASSERT_TRUE(ip_v4 == nullptr);
	}
}

