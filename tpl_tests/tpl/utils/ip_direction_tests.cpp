#include <gtest/gtest.h>
#include <tpl/utils/ip_direction.hpp>

using namespace tpl::utils;

namespace tpl_tests
{
	TEST(ip_direction_tests, ctor_sets_src_and_dst_correctly)
	{
		auto src = boost::asio::ip::address::from_string("192.168.1.1");
		auto dst = boost::asio::ip::address::from_string("192.168.1.2");

		ip_direction d(src, dst);
		ASSERT_EQ(src, d.source());
		ASSERT_EQ(dst, d.destination());
	}

	TEST(ip_direction_tests, can_create_ip_direction_from_string)
	{
		auto d = ip_direction::from_string("192.168.1.1", "127.0.0.1");

		ASSERT_EQ(d.source().to_string(), "192.168.1.1");
		ASSERT_EQ(d.destination().to_string(), "127.0.0.1");
	}

	TEST(ip_direction_tests, can_get_reverce_direction)
	{
		auto src = boost::asio::ip::address::from_string("192.168.1.1");
		auto dst = boost::asio::ip::address::from_string("192.168.1.2");

		ip_direction d(src, dst);
		auto r = d.reverce();

		ASSERT_EQ(d.source(), r.destination());
		ASSERT_EQ(d.destination(), r.source());
	}

	TEST(ip_direction_tests, reverce_function_works_properly)
	{
		ASSERT_TRUE(ip_direction::from_string("192.168.1.1", "127.0.0.1")
			.is_reverce_to(
				ip_direction::from_string("127.0.0.1", "192.168.1.1")));

		ASSERT_FALSE(ip_direction::from_string("192.168.1.1", "127.0.0.1")
			.is_reverce_to(
				ip_direction::from_string("192.168.1.1", "127.0.0.1")));
	}
}

