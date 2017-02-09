#include <gtest/gtest.h>
#include <vector>
#include "test_helper.hpp"
#include <tpl/packets/transport/tcp_packet.hpp>
#include <tpl/packets/data_link/ethernet_packet.hpp>
using namespace std;
using namespace tpl;
using namespace packets;

namespace tpl_tests
{
	TEST(tcp_packet_tests, can_get_tcp_parameters)
	{
		auto tcp = test_helper.get_packet<tcp_packet>("eth_ip_tcp.pcap",
		                                              [](const byte_array& bytes)
		                                              {
			                                              auto eth = ethernet_packet(bytes);
			                                              auto ip = ip_packet(packets::placeholders::parent_packet = eth);
			                                              return tcp_packet(packets::placeholders::parent_packet = ip);
		                                              },
		                                              0);

		ASSERT_EQ(uint16_t(2096), tcp.src_port());
		ASSERT_EQ(uint16_t(80), tcp.dst_port());
		ASSERT_EQ(uint32_t(2573193080), tcp.seq_number());
		ASSERT_EQ(uint32_t(0), tcp.ack_number());
		ASSERT_EQ(uint8_t(28), tcp.header_length());

		ASSERT_FALSE(tcp.flags().nonce);
		ASSERT_FALSE(tcp.flags().cwr);
		ASSERT_FALSE(tcp.flags().ecn_echo);
		ASSERT_FALSE(tcp.flags().urg);
		ASSERT_FALSE(tcp.flags().ack);
		ASSERT_FALSE(tcp.flags().push);
		ASSERT_FALSE(tcp.flags().reset);
		ASSERT_TRUE(tcp.flags().syn);
		ASSERT_FALSE(tcp.flags().fin);

		ASSERT_EQ(uint16_t(65535), tcp.windows_size());
		ASSERT_EQ(uint16_t(0xf98f), tcp.checksum());
		ASSERT_EQ(uint16_t(0), tcp.urgent_pointer());

		ASSERT_EQ(size_t(28), tcp.header_bytes().size());
		ASSERT_EQ(size_t(0), tcp.payload_bytes().size());

		vector<tcp_option> options;
		for (auto const& op : tcp.options())
		{
			options.push_back(op);
		}
		ASSERT_EQ(size_t(4), options.size());
	}

	TEST(tcp_packet_tests, test_tcp_flags_struct)
	{
		ASSERT_TRUE(tcp_flags(0x0001).fin);
		ASSERT_TRUE(tcp_flags(0x0002).syn);
		ASSERT_TRUE(tcp_flags(0x0004).reset);
		ASSERT_TRUE(tcp_flags(0x0008).push);
		ASSERT_TRUE(tcp_flags(0x0010).ack);
		ASSERT_TRUE(tcp_flags(0x0020).urg);
		ASSERT_TRUE(tcp_flags(0x0040).ecn_echo);
		ASSERT_TRUE(tcp_flags(0x0080).cwr);
		ASSERT_TRUE(tcp_flags(0x0100).nonce);
	}

	TEST(tcp_packet_tests, process_some_malformed_ip)
	{
		byte_array bytes = {0x45, 0x00, 0x00, 0x10, 0x34, 0x4d, 0x00, 0x00, 0x3b, 0x06, 0xe2, 0x39, 0x40, 0x0f, 0x71, 0xba,
			0xd5, 0x2f, 0xe2, 0x50, 0x01, 0xbb, 0xe7, 0x15, 0xaf, 0xae, 0xad, 0xcb, 0xa7, 0xdc, 0x81, 0x2b,
			0x50, 0x10, 0x00, 0xf5, 0xd6, 0x42, 0x00, 0x00, 0x00, 0x00, 0x93, 0xf9, 0x29, 0xaa};
		auto ip = ip_packet(bytes);
		ASSERT_THROW(auto tcp = tcp_packet(tpl::packets::placeholders::parent_packet = ip),
			std::exception);
	}
}
