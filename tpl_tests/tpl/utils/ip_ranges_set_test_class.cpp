#include <gtest/gtest.h>
#include <tpl/utils/ip_range_set.hpp>
#include <list>
using namespace tpl::utils;

namespace tpl_tests
{
	TEST(ip_ranges_set_tests, ctor_test)
	{
		ip_range_set range_set;
		range_set.add(ip_address_range("127.0.0.12", "127.0.0.17"));
		range_set.add(ip_address_range("127.0.0.7", "127.0.0.12"));
		range_set.add(ip_address_range("127.0.0.5"));
		range_set.add(ip_address_range("127.0.0.1", "127.0.0.10"));
	}

	TEST(ip_ranges_set_tests, can_find_address_that_is_first_of_some_range)
	{
		ip_range_set range_set;
		range_set.add(ip_address_range("127.0.0.1", "127.0.0.2"));
		ASSERT_TRUE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.1")));
	}

	TEST(ip_ranges_set_tests, can_find_address_that_is_last_of_some_range)
	{
		ip_range_set range_set;
		range_set.add(ip_address_range("127.0.0.1", "127.0.0.2"));
		ASSERT_TRUE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.2")));
	}

	TEST(ip_ranges_set_tests, can_find_address_that_is_in_middle_of_some_range)
	{
		ip_range_set range_set;
		range_set.add(ip_address_range("127.0.0.1", "127.0.0.3"));
		ASSERT_TRUE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.2")));
	}

	TEST(ip_ranges_set_tests, can_find_address_in_set_of_intersecting_ranges)
	{
		//////////////////////////////////////////////////////////////////////////
		// 127.0.0.1    127.0.0.5   127.0.0.7   127.0.0.10   127.0.0.12    127.0.0.17
		//      [====================================]
		//                                                       [==============]
		//                              [=========================]
		//                  [=]
		ip_range_set range_set;
		range_set.add(ip_address_range("127.0.0.12", "127.0.0.17"));
		range_set.add(ip_address_range("127.0.0.7", "127.0.0.12"));
		range_set.add(ip_address_range("127.0.0.5"));
		range_set.add(ip_address_range("127.0.0.1", "127.0.0.10"));

		ASSERT_FALSE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.0")));
		ASSERT_TRUE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.1")));
		ASSERT_TRUE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.5")));
		ASSERT_TRUE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.7")));
		ASSERT_TRUE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.10")));
		ASSERT_TRUE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.12")));
		ASSERT_TRUE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.15")));
		ASSERT_TRUE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.17")));
		ASSERT_FALSE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.18")));
	}

	TEST(ip_ranges_set_tests, can_find_single_matched_range)
	{
		ip_range_set range_set;
		range_set.add(ip_address_range("127.0.0.1", "127.0.0.2"));

		auto matched = range_set.find_includings(boost::asio::ip::address::from_string("127.0.0.1"));

		ASSERT_EQ(size_t(1), matched.size());
		ASSERT_EQ(ip_address_range("127.0.0.1", "127.0.0.2"), *matched.begin());
	}

	TEST(ip_ranges_set_tests, can_find_matched_address)
	{
		ip_range_set range_set;
		range_set.add(ip_address_range("127.0.0.1"));

		auto matched = range_set.find_includings(boost::asio::ip::address::from_string("127.0.0.1"));

		ASSERT_EQ(size_t(1), matched.size());
		ASSERT_EQ(ip_address_range("127.0.0.1"), *matched.begin());
	}

	TEST(ip_ranges_set_tests, can_find_several_matched_range)
	{
		ip_range_set range_set;
		range_set.add(ip_address_range("127.0.0.1", "127.0.0.20"));
		range_set.add(ip_address_range("127.0.0.3", "127.0.0.23"));
		range_set.add(ip_address_range("127.0.0.6", "127.0.0.26"));
		range_set.add(ip_address_range("127.0.0.9", "127.0.0.29"));
		range_set.add(ip_address_range("127.0.0.12", "127.0.0.32"));

		auto matched = range_set.find_includings(boost::asio::ip::address::from_string("127.0.0.0"));
		ASSERT_EQ(size_t(0), matched.size());
		matched = range_set.find_includings(boost::asio::ip::address::from_string("127.0.0.2"));
		ASSERT_EQ(size_t(1), matched.size());
		matched = range_set.find_includings(boost::asio::ip::address::from_string("127.0.0.4"));
		ASSERT_EQ(size_t(2), matched.size());
		matched = range_set.find_includings(boost::asio::ip::address::from_string("127.0.0.7"));
		ASSERT_EQ(size_t(3), matched.size());
		matched = range_set.find_includings(boost::asio::ip::address::from_string("127.0.0.10"));
		ASSERT_EQ(size_t(4), matched.size());
		matched = range_set.find_includings(boost::asio::ip::address::from_string("127.0.0.13"));
		ASSERT_EQ(size_t(5), matched.size());
		matched = range_set.find_includings(boost::asio::ip::address::from_string("127.0.0.21"));
		ASSERT_EQ(size_t(4), matched.size());
		matched = range_set.find_includings(boost::asio::ip::address::from_string("127.0.0.24"));
		ASSERT_EQ(size_t(3), matched.size());
		matched = range_set.find_includings(boost::asio::ip::address::from_string("127.0.0.27"));
		ASSERT_EQ(size_t(2), matched.size());
		matched = range_set.find_includings(boost::asio::ip::address::from_string("127.0.0.30"));
		ASSERT_EQ(size_t(1), matched.size());
		matched = range_set.find_includings(boost::asio::ip::address::from_string("127.0.0.33"));
		ASSERT_EQ(size_t(0), matched.size());
	}

	TEST(ip_ranges_set_tests, can_find_simple_range_intersection)
	{
		ip_range_set range_set;
		range_set.add(ip_address_range("127.0.0.1", "127.0.0.20"));

		auto t = range_set.find_intersections(ip_address_range("127.0.0.2", "127.0.0.19"));
		ASSERT_EQ(size_t(1), t.size());
		ASSERT_EQ(ip_address_range("127.0.0.1", "127.0.0.20"), *t.begin());
	}

	TEST(ip_ranges_set_tests, can_find_intersection_if_intersects_by_start)
	{
		ip_range_set range_set;
		range_set.add(ip_address_range("127.0.0.1", "127.0.0.20"));

		auto t = range_set.find_intersections(ip_address_range("127.0.0.19", "127.0.0.22"));
		ASSERT_EQ(size_t(1), t.size());
		ASSERT_EQ(ip_address_range("127.0.0.1", "127.0.0.20"), *t.begin());
	}

	TEST(ip_ranges_set_tests, can_find_intersection_if_intersects_by_end)
	{
		ip_range_set range_set;
		range_set.add(ip_address_range("127.0.0.10", "127.0.0.20"));

		auto t = range_set.find_intersections(ip_address_range("127.0.0.1", "127.0.0.11"));
		ASSERT_EQ(size_t(1), t.size());
		ASSERT_EQ(ip_address_range("127.0.0.10", "127.0.0.20"), *t.begin());
	}

	TEST(ip_ranges_set_tests, can_find_intersection_if_convers_several_ranges)
	{
		ip_range_set range_set;
		range_set.add(ip_address_range("127.0.0.10", "127.0.0.20"));
		range_set.add(ip_address_range("127.0.0.30", "127.0.0.40"));

		auto t = range_set.find_intersections(ip_address_range("127.0.0.1", "127.0.0.50"));
		ASSERT_EQ(size_t(2), t.size());
		ASSERT_EQ(ip_address_range("127.0.0.10", "127.0.0.20"), *t.begin());
		ASSERT_EQ(ip_address_range("127.0.0.30", "127.0.0.40"), *(++t.begin()));
	}

	TEST(ip_ranges_set_tests, can_find_intersection_if_partly_intersects_several_ranges)
	{
		ip_range_set range_set;
		range_set.add(ip_address_range("127.0.0.10", "127.0.0.20"));
		range_set.add(ip_address_range("127.0.0.30", "127.0.0.40"));

		auto t = range_set.find_intersections(ip_address_range("127.0.0.15", "127.0.0.35"));
		ASSERT_EQ(size_t(2), t.size());
		ASSERT_EQ(ip_address_range("127.0.0.10", "127.0.0.20"), *t.begin());
		ASSERT_EQ(ip_address_range("127.0.0.30", "127.0.0.40"), *(++t.begin()));
	}

	TEST(ip_ranges_set_tests, can_find_intersection_if_intersects_one_of_intersecting_ranges_in_the_end)
	{
		ip_range_set range_set;
		range_set.add(ip_address_range("127.0.0.10", "127.0.0.20"));
		range_set.add(ip_address_range("127.0.0.15", "127.0.0.25"));

		auto t = range_set.find_intersections(ip_address_range("127.0.0.22", "127.0.0.25"));
		ASSERT_EQ(size_t(1), t.size());
		ASSERT_EQ(ip_address_range("127.0.0.15", "127.0.0.25"), *t.begin());
	}

	TEST(ip_ranges_set_tests, can_find_intersection_if_intersects_one_of_intersecting_ranges_in_the_beginning)
	{
		ip_range_set range_set;
		range_set.add(ip_address_range("127.0.0.10", "127.0.0.20"));
		range_set.add(ip_address_range("127.0.0.15", "127.0.0.25"));

		auto t = range_set.find_intersections(ip_address_range("127.0.0.1", "127.0.0.11"));
		ASSERT_EQ(size_t(1), t.size());
		ASSERT_EQ(ip_address_range("127.0.0.10", "127.0.0.20"), *t.begin());
	}

	TEST(ip_ranges_set_tests, can_remove_whole_range)
	{
		ip_range_set range_set;
		range_set.add(ip_address_range("127.0.0.10", "127.0.0.20"));

		ASSERT_TRUE(range_set.remove(ip_address_range("127.0.0.10", "127.0.0.20")));

		ASSERT_FALSE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.10")));
		ASSERT_FALSE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.15")));
		ASSERT_FALSE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.20")));
	}

	TEST(ip_ranges_set_tests, can_remove_of_one_intersecting_ranges)
	{
		ip_range_set range_set;
		range_set.add(ip_address_range("127.0.0.10", "127.0.0.20"));
		range_set.add(ip_address_range("127.0.0.15", "127.0.0.25"));

		ASSERT_TRUE(range_set.remove(ip_address_range("127.0.0.10", "127.0.0.20")));

		ASSERT_FALSE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.10")));
		ASSERT_FALSE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.14")));
		ASSERT_TRUE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.15")));
		ASSERT_TRUE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.20")));
		ASSERT_TRUE(range_set.contains(boost::asio::ip::address::from_string("127.0.0.25")));
	}
}

