#include <gtest/gtest.h>
#include <tpl/packets/packet_metainfo.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/any.hpp>
#include <boost/property_map/property_map.hpp>

using namespace tpl::packets;

namespace tpl_tests
{
	TEST(packet_metainfo_tests, can_set_and_get_generic_metavalue)
	{
		boost::posix_time::ptime target_time = boost::posix_time::second_clock::local_time();

		class some_time_tag : public metavalue_tag<boost::posix_time::ptime> {};

		packet_metainfo metainfo;
		metainfo.set<some_time_tag>(target_time);

		auto restored = metainfo.get<some_time_tag>();
		ASSERT_TRUE(restored.is_initialized());
		ASSERT_EQ(target_time, *restored);
	}

	TEST(packet_metainfo_tests, can_set_and_get_embedded_metavalue)
	{
		auto some_ip = boost::asio::ip::address::from_string("127.0.0.134");
		packet_metainfo metainfo;
		metainfo.set<source_ip_tag>(some_ip);

		auto restored_ip = metainfo.get<source_ip_tag>();
		ASSERT_TRUE(restored_ip.is_initialized());
		ASSERT_EQ(some_ip, *restored_ip);
	}

	TEST(packet_metainfo_tests, can_set_and_get_both_embedded_and_generic_metavalue)
	{
		class some_time_tag : public metavalue_tag<boost::posix_time::ptime> {};

		packet_metainfo metainfo;

		boost::posix_time::ptime target_time = boost::posix_time::second_clock::local_time();
		metainfo.set<some_time_tag>(target_time);

		auto some_ip = boost::asio::ip::address::from_string("127.0.0.134");
		metainfo.set<source_ip_tag>(some_ip);

		auto restored_time = metainfo.get<some_time_tag>();
		ASSERT_TRUE(restored_time.is_initialized());
		ASSERT_EQ(target_time, *restored_time);


		auto restored_ip = metainfo.get<source_ip_tag>();
		ASSERT_TRUE(restored_ip.is_initialized());
		ASSERT_EQ(some_ip, *restored_ip);
	}
}

