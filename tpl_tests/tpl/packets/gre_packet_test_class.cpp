#include <gtest/gtest.h>
#include <tpl/formats/pcap/pcap_file.hpp>
#include <tpl/packets/gre_packet.hpp>
#include "test_helper.hpp"
using namespace tpl;
using namespace tpl::packets;
using namespace tpl::formats;


namespace tpl_tests
{
	TEST(gre_packet_tests, can_get_base_parameters)
	{
		gre_packet_base p = test_helper.get_packet<gre_packet_base>("gre_and_4over6.pcap",
		                                                            [](const byte_array& bytes)
		                                                            {
			                                                            return gre_packet_base(bytes.suffix(78));
		                                                            }, 0);

		ASSERT_FALSE(p.flags().checksum_present());
		ASSERT_FALSE(p.flags().routing_present());
		ASSERT_TRUE(p.flags().key_present());
		ASSERT_TRUE(p.flags().seq_number_present());
		ASSERT_FALSE(p.flags().strict_source_route());
		ASSERT_EQ(uint8_t(0), p.flags().recursion_control());
		ASSERT_TRUE(p.flags().ack_number_present());
		ASSERT_EQ(gre_version::enhanced_gre, p.flags().version());
		ASSERT_EQ(gre_payload_protocol::ppp, p.payload_protocol());
	}

	TEST(gre_packet_tests, can_cast_from_gre_base_to_derived)
	{
		gre_packet_base p = test_helper.get_packet<gre_packet_base>("gre_and_4over6.pcap",
		                                                            [](const byte_array& bytes)
		                                                            {
			                                                            return gre_packet_base(bytes.suffix(78));
		                                                            }, 0);

		auto enchased = p.as<enhanced_gre_packet>();
	}

	TEST(gre_packet_tests, throws_bad_cast_if_invalid_cast)
	{
		gre_packet_base p = test_helper.get_packet<gre_packet_base>("gre_and_4over6.pcap",
		                                                            [](const byte_array& bytes)
		                                                            {
			                                                            return gre_packet_base(bytes.suffix(78));
		                                                            }, 0);
		ASSERT_THROW(p.as<gre_packet>(), bad_cast_exception);
	}

	TEST(gre_packet_tests, check_enchased_gre_parameters)
	{
		gre_packet_base p = test_helper.get_packet<gre_packet_base>("gre_and_4over6.pcap",
		                                                            [](const byte_array& bytes)
		                                                            {
			                                                            return gre_packet_base(bytes.suffix(78));
		                                                            }, 0);

		enhanced_gre_packet enhanced = p.as<enhanced_gre_packet>();

		ASSERT_FALSE(enhanced.flags().checksum_present());
		ASSERT_FALSE(enhanced.flags().routing_present());
		ASSERT_TRUE(enhanced.flags().key_present());
		ASSERT_TRUE(enhanced.flags().seq_number_present());
		ASSERT_FALSE(enhanced.flags().strict_source_route());
		ASSERT_TRUE(enhanced.flags().ack_number_present());
		ASSERT_EQ(uint8_t(0), enhanced.flags().recursion_control());
		ASSERT_EQ(gre_version::enhanced_gre, enhanced.flags().version());
		ASSERT_EQ(gre_payload_protocol::ppp, enhanced.payload_protocol());
		ASSERT_EQ(uint16_t(103), enhanced.payload_length());
		ASSERT_EQ(uint16_t(6016), enhanced.call_id());
		ASSERT_EQ(uint32_t(430001), enhanced.seq_number());
		ASSERT_EQ(uint32_t(539254), enhanced.ack_number());

		ASSERT_EQ(size_t(16), enhanced.header_bytes().size());
		ASSERT_EQ(size_t(103), enhanced.payload_bytes().size());
	}
}

