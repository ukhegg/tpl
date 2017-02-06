#include <gtest/gtest.h>
#include <tpl/utils/ip_address_range.hpp>
using namespace tpl::utils;
using namespace boost::asio::ip;


namespace tpl_tests
{
	TEST(ip_address_range_tests, throws_if_ctor_arguments_not_the_same_type)
	{
		ASSERT_THROW(ip_address_range("127.0.0.1", "fe80::20c:29ff:fe10:cf00"),
			std::invalid_argument);
	}

	TEST(ip_address_range_tests, creates_valid_if_parameters_order_invalid)
	{
		ASSERT_TRUE(ip_address_range("127.0.0.10", "127.0.0.0").contains("127.0.0.5"));
	}

	TEST(ip_address_range_tests, check_v4_address_range)
	{
		ASSERT_TRUE(ip_address_range("127.0.0.1", "127.0.0.10").contains("127.0.0.1"));
		ASSERT_TRUE(ip_address_range("127.0.0.1", "127.0.0.10").contains("127.0.0.5"));
		ASSERT_TRUE(ip_address_range("127.0.0.1", "127.0.0.10").contains("127.0.0.10"));

		ASSERT_FALSE(ip_address_range("127.0.0.1", "127.0.0.10").contains("127.0.0.0"));
		ASSERT_FALSE(ip_address_range("127.0.0.1", "127.0.0.10").contains("127.0.0.11"));

		ASSERT_TRUE(ip_address_range("127.0.0.0", "127.0.1.0").contains("127.0.0.0"));
		ASSERT_TRUE(ip_address_range("127.0.0.0", "127.0.1.10").contains("127.0.1.0"));
		ASSERT_TRUE(ip_address_range("127.0.0.0", "127.0.1.0").contains("127.0.0.11"));
	}

	TEST(ip_address_range_tests, check_v6_address_range)
	{
		ASSERT_TRUE(ip_address_range("fe80::20c:29ff:fe10:cf00", "fe80::20c:29ff:fe10:cf0f").contains("fe80::20c:29ff:fe10:cf00"));
		ASSERT_TRUE(ip_address_range("fe80::20c:29ff:fe10:cf00", "fe80::20c:29ff:fe10:cf0f").contains("fe80::20c:29ff:fe10:cf0e"));
		ASSERT_TRUE(ip_address_range("fe80::20c:29ff:fe10:cf00", "fe80::20c:29ff:fe10:cf0f").contains("fe80::20c:29ff:fe10:cf0f"));

		ASSERT_FALSE(ip_address_range("fe80::20c:29ff:fe10:cf01", "fe80::20c:29ff:fe10:cf0f").contains("fe80::20c:29ff:fe10:cf00"));
		ASSERT_FALSE(ip_address_range("fe80::20c:29ff:fe10:cf00", "fe80::20c:29ff:fe10:cf0f").contains("fe80::20c:29ff:fe10:cf10"));

		ASSERT_TRUE(ip_address_range("fe80::20c:29ff:fe10:cf01", "fe80::21c:29ff:fe10:cf0f").contains("fe80::20c:29ff:fe10:cf01"));
		ASSERT_TRUE(ip_address_range("fe80::20c:29ff:fe10:cf01", "fe80::21c:29ff:fe10:cf0f").contains("fe80::20c:2aff:fe10:cf01"));
		ASSERT_TRUE(ip_address_range("fe80::20c:29ff:fe10:cf01", "fe80::21c:29ff:fe10:cf0f").contains("fe80::21c:29ff:fe10:cf0f"));
	}

	TEST(ip_address_range_tests, check_v4_doesnt_contain_v6)
	{
		ASSERT_FALSE(ip_address_range("127.0.0.1", "127.0.0.10").contains("fe80::20c:29ff:fe10:cf00"));
	}

	TEST(ip_address_range_tests, check_v6_doesnt_contain_v4)
	{
		ASSERT_FALSE(ip_address_range("fe80::20c:29ff:fe10:cf00", "fe80::20c:29ff:fe10:cf0f").contains("127.0.0.1"));
	}

	TEST(ip_address_range_tests, check_range_containing)
	{
		ip_address_range range1("127.0.0.1", "127.0.0.10");
		ip_address_range range2("127.0.0.0", "127.0.0.11");

		ASSERT_TRUE(range2.contains(range1));
		ASSERT_FALSE(range1.contains(range2));
	}

	TEST(ip_address_range_tests, doesnt_contain_range_if_intersect_partly)
	{
		ip_address_range range1("127.0.0.0", "127.0.0.11");
		ip_address_range range2("127.0.0.1", "127.0.0.12");

		ASSERT_FALSE(range2.contains(range1));
		ASSERT_FALSE(range1.contains(range2));
	}

	TEST(ip_address_range_tests, check_v6_range_doesnt_contain_v4_range)
	{
		ip_address_range v4Range("127.0.0.0", "127.0.0.11");
		ip_address_range v6Range("fe80::20c:29ff:fe10:cf00", "fe80::20c:29ff:fe10:cf0f");

		ASSERT_FALSE(v4Range.contains(v6Range));
		ASSERT_FALSE(v6Range.contains(v4Range));
	}
}

