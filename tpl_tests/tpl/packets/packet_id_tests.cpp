//
// Created by ukhegg on 06.11.2016.
//

#include <gtest/gtest.h>
#include <tpl/object_id.hpp>
#include <tpl/packets/ip_v4_packet.hpp>
#include <tpl/packets/gre_packet.hpp>
using namespace tpl;
using namespace packets;

namespace tpl_tests
{
	TEST(packet_id_tests, packet_id_of_parent_packet_is_less_than_packet_id_of_derived)
	{
		auto ip_packet_id = objects_id_repository::id_of<ip_packet>();
		auto ip_v4_packet_id = objects_id_repository::id_of<ip_v4_packet>();

		ASSERT_TRUE(ip_packet_id < ip_v4_packet_id);
	}

	TEST(packet_id_tests, packet_id_of_packet_is_lelf_less)
	{
		auto ip_packet_id1 = objects_id_repository::id_of<ip_packet>();
		auto ip_packet_id2 = objects_id_repository::id_of<ip_packet>();


		ASSERT_FALSE(ip_packet_id1 < ip_packet_id2);
		ASSERT_FALSE(ip_packet_id2 < ip_packet_id1);
	}

	TEST(packet_id_tests, trying_to_compare_different_hierarchies_packet_ids_throws_exception)
	{
		auto ip_packet_id1 = objects_id_repository::id_of<ip_packet>();
		auto ip_packet_id2 = objects_id_repository::id_of<gre_packet>();

		ASSERT_THROW(ip_packet_id1 < ip_packet_id2, std::invalid_argument);
	}
}
