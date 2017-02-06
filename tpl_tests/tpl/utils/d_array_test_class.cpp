#include <gtest/gtest.h>
#include <tpl/utils/d_array.hpp>
#include <sstream>
using namespace std;
using namespace tpl::utils;

namespace tpl_tests
{
	TEST(d_array_tests, d_array_empty_ctor_test)
	{
		d_array<int> ar;
		ASSERT_EQ((size_t)0, ar.size());
		ASSERT_EQ(ar.data(), nullptr);
	}

	TEST(d_array_tests, d_array_ctor_with_size_test)
	{
		d_array<uint8_t> ar(100);
		ASSERT_EQ((size_t)100, ar.size());
		ASSERT_NE(ar.data(), nullptr);
	}

	TEST(d_array_tests, d_array_ctor_with_allocate_native_array)
	{
		int* ints = new int[100];
		d_array<int> ar(ints, 100);
		ASSERT_EQ((size_t)100, ar.size());
		ASSERT_EQ(ints, ar.data());
	}

	TEST(d_array_tests, d_array_assign_operator_test)
	{
		d_array<int> ar1(100);
		d_array<int> ar2(200);
		ar1 = ar2;
		ASSERT_EQ((size_t)200, ar1.size());
		ASSERT_EQ(ar1.data(), ar2.data());
	}

	TEST(d_array_tests, d_array_read_indexer_test)
	{
		int* ints = new int[255];
		for(int i = 0 ;i < 255 ;++i)
		{
			ints[i] = i;
		}
		d_array<int> ar(ints, 255);
		for(int i = 0 ;i < 255 ;++i)
		{
			ASSERT_EQ(i, ar[i]);
		}
	}

	TEST(d_array_tests, d_array_write_indexer_test)
	{
		d_array<int> ar(255);
		for(int i = 0 ;i < 255 ;++i)
		{
			ar[i] = i;
			ASSERT_EQ(i, ar[i]);
		}
	}

	TEST(d_array_tests, d_array_throw_out_or_range_if_indexer_out_of_range)
	{
		auto t = []()
				{
					d_array<int> ar(255);
					ar[255] = 100;
				};
		ASSERT_THROW(t(), std::out_of_range);
	}

	TEST(d_array_tests, d_array_throw_out_or_range_if_indexer_negative)
	{
		auto t = []()
				{
					d_array<int> ar(255);
					ar[static_cast<size_t>(-1)] = 100;
				};
		ASSERT_THROW(t(), std::out_of_range);
	}

	TEST(d_array_tests, d_array_holds_array_if_any_pointer_left)
	{
		auto data = new int[100];
		d_array<int> ar(data, 100);

		d_array<int> b[1];
		b[0] = ar;

		ar = d_array<int>(100);

		d_array<int> c = b[0];
		ASSERT_EQ(data, c.data());
	}

	TEST(d_array_tests, d_array_simple_iterator_test)
	{
		d_array<int> ar1(100);
		d_array<int> ar2(100);

		for(int i = 0 ;i < 100 ;i++)
		{
			ar1[i] = i;
		}

		std::copy(ar1.begin(), ar1.end(), ar2.begin());
		for(int i = 0 ;i < 100 ;i++)
		{
			ASSERT_EQ(i, ar2[i]);
		}
	}

	TEST(d_array_tests, d_array_can_get_prefix)
	{
		d_array<int> ar1(100);
		for(int i = 0 ;i < 100 ;i++)
		{
			ar1[i] = i;
		}

		d_array<int> prefix = ar1.prefix(66);

		ASSERT_EQ(size_t(66), prefix.size());
		ASSERT_EQ(0, prefix[0]);
		ASSERT_EQ(65, prefix[65]);
	}

	TEST(d_array_tests, d_array_throws_if_prefix_too_long)
	{
		auto t = []()
				{
					d_array<int> ar(255);
					d_array<int> prefix = ar.prefix(256);
				};
		ASSERT_THROW(t(), std::out_of_range);
	}

	TEST(d_array_tests, d_array_can_get_suffix)
	{
		d_array<int> ar1(100);
		for(int i = 0 ;i < 100 ;i++)
		{
			ar1[i] = i;
		}

		d_array<int> suffix = ar1.suffix(66);

		ASSERT_EQ(size_t(100 - 66), suffix.size());
		ASSERT_EQ(66, suffix[0]);
	}

	TEST(d_array_tests, d_array_throws_if_suffix_too_long)
	{
		auto t = []()
				{
					d_array<int> ar(255);
					d_array<int> suffix = ar.suffix(256);
				};
		ASSERT_THROW(t(), std::out_of_range);
	}

	TEST(d_array_tests, d_array_can_get_middle)
	{
		d_array<int> ar1(100);
		for(int i = 0 ;i < 100 ;i++)
		{
			ar1[i] = i;
		}

		d_array<int> middle = ar1.middle(5, 10);

		ASSERT_EQ(size_t(10), middle.size());
		ASSERT_EQ(5, middle[0]);
		ASSERT_EQ(14, middle[9]);
	}

	TEST(d_array_tests, d_array_throws_if_middle_offset_to_big)
	{
		auto t = []()
				{
					d_array<int> ar(255);
					d_array<int> suffix = ar.middle(256, 1);
				};
		ASSERT_THROW(t(), std::out_of_range);
	}

	TEST(d_array_tests, d_array_throws_if_middle_end_out_of_range)
	{
		auto t = []()
				{
					d_array<int> ar(255);
					d_array<int> suffix = ar.middle(200, 56);
				};
		ASSERT_THROW(t(), std::out_of_range);
	}

	TEST(d_array_tests, d_array_keeps_array_if_any_middle_alive)
	{
		d_array<int> ar(255);
		int* arBegin = ar.data();

		for(int i = 0 ;i < 100 ;i++)
		{
			ar = ar.middle(1, ar.size() - 2);
		}

		ASSERT_EQ((size_t)55, ar.size());
		ASSERT_EQ(arBegin + 100, ar.data());
	}

	TEST(d_array_tests, d_array_can_get_copy)
	{
		d_array<int> ar1(100);
		for(int i = 0 ;i < 100 ;i++) ar1[i] = i;
		d_array<int> c = ar1.middle(5, 10).clone();

		ASSERT_NE(ar1.middle(5, 10).data(), c.data());
		ASSERT_EQ(size_t(10), c.size());
		ASSERT_EQ(5, c[0]);
		ASSERT_EQ(14, c[9]);
	}

	TEST(d_array_tests, d_array_creates_an_independent_copy)
	{
		d_array<int> c;
		{
			d_array<int> ar1(100);
			for(int i = 0 ;i < 100 ;i++) ar1[i] = i;
			c = ar1.middle(5, 10).clone();
		}

		ASSERT_EQ(size_t(10), c.size());
		ASSERT_NE(c.data(), nullptr);
	}

	TEST(d_array_tests, d_array_can_merge_2_arrays)
	{
		d_array<int> first({1, 2}), second({3, 4});

		d_array<int> result = first.merge(second);
		ASSERT_EQ(size_t(4), result.size());

		ASSERT_EQ(1, result[0]);
		ASSERT_EQ(2, result[1]);
		ASSERT_EQ(3, result[2]);
		ASSERT_EQ(4, result[3]);
	}

	TEST(d_array_tests, d_array_can_merge_several_arrays)
	{
		d_array<int> first({1, 2}),
				second({3, 4}),
				third({5, 6}),
				fourth({7, 8});

		d_array<int> result = first.merge(second).merge(third).merge(fourth);

		ASSERT_EQ(size_t(8), result.size());
		for(int i = 0 ;i < 8 ;++i)
		{
			ASSERT_EQ(i + 1, result[i]);
		}
	}
}

