#include <gtest/gtest.h>
#include <tpl/utils/data_size.hpp>
#include <sstream>
using namespace std;
using namespace tpl;
using namespace utils;

namespace tpl_tests
{
	TEST(data_size_tests, can_add)
	{
		data_size s(1024);
		data_size s2(1023);
		ASSERT_EQ(data_size(2047), s + s2);
	}

	TEST(data_size_tests, can_subtract)
	{
		data_size s(1024);
		data_size s2(1023);
		ASSERT_EQ(data_size(1), s - s2);
	}

	TEST(data_size_tests, can_multiply)
	{
		data_size s(1024);
		ASSERT_EQ(data_size(2048), s*int64_t(2));
	}

	TEST(data_size_tests, can_devide)
	{
		data_size s(4096);
		ASSERT_EQ(data_size(2048), s / int64_t(2));
	}

	TEST(data_size_tests, can_get_order)
	{
		data_size s(1, data_size_unit::byte);
		ASSERT_EQ(data_size_unit::byte, s.order());

		s = data_size(1, data_size_unit::kilobyte);
		ASSERT_EQ(data_size_unit::kilobyte, s.order());

		s = data_size(1, data_size_unit::megabyte);
		ASSERT_EQ(data_size_unit::megabyte, s.order());

		s = data_size(1, data_size_unit::gigabyte);
		ASSERT_EQ(data_size_unit::gigabyte, s.order());

		s = data_size(1, data_size_unit::terabyte);
		ASSERT_EQ(data_size_unit::terabyte, s.order());
	}

	TEST(data_size_tests, can_get_correct_total_utils)
	{
		data_size s(1024 * 1024 + 1024 + 3);
		ASSERT_EQ(int64_t(1), s.total(data_size_unit::megabyte));
		ASSERT_EQ(int64_t(1025), s.total(data_size_unit::kilobyte));
		ASSERT_EQ(int64_t(1024 * 1024 + 1024 + 3), s.total(data_size_unit::byte));
	}

	TEST(data_size_tests, can_get_correct_utils_count)
	{
		data_size s(1024 * 1024 + 1024 + 3);
		ASSERT_EQ(int64_t(1), s.count(data_size_unit::megabyte));
		ASSERT_EQ(int64_t(1), s.count(data_size_unit::kilobyte));
		ASSERT_EQ(int64_t(3), s.count(data_size_unit::byte));
	}

	TEST(data_size_tests, can_get_correct_string)
	{
		auto s = data_size::bytes(0);
		ASSERT_EQ(string("0 B"), s.to_string<char>());

		s = data_size::megabytes(3) + data_size::kilobytes(123);
		ASSERT_EQ(string("3.12 Mb"), s.to_string<char>());

		s = data_size::megabytes(23) + data_size::kilobytes(123);
		ASSERT_EQ(string("23.1 Mb"), s.to_string<char>());

		s = data_size::kilobytes(123);
		ASSERT_EQ(string("123 Kb"), s.to_string<char>());

		s = data_size::kilobytes(123) + data_size::bytes(25);
		ASSERT_EQ(string("123 Kb"), s.to_string<char>());

		s = data_size::bytes(25);
		ASSERT_EQ(string("25 B"), s.to_string<char>());

		s = data_size::megabytes(1010) + data_size::bytes(1);
		ASSERT_EQ(string("1010 Mb"), s.to_string<char>());
	}

	TEST(data_size_tests, can_get_corrent_string_when_outputs_in_a_stream)
	{
		auto s = data_size::bytes(0);
		std::ostringstream oss;
		oss << s;
		ASSERT_EQ(string("0 B"), oss.str());
	}

	TEST(data_size_tests, can_get_corrent_string_when_outputs_in_a_stream2)
	{
		auto s = data_size::megabytes(1024) + data_size::bytes(1);
		std::ostringstream oss;
		oss << s;
		ASSERT_EQ(string("1 Gb"), oss.str());
	}

	TEST(data_size_tests, can_get_corrent_string_when_outputs_in_a_stream3)
	{
		auto s = data_size::megabytes(1023) + data_size::bytes(1);
		std::ostringstream oss;
		oss << s;
		ASSERT_EQ(string("1023 Mb"), oss.str());
	}
}

