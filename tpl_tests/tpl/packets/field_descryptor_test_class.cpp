#include <gtest/gtest.h>
#include <tpl/packets/field_descriptor.hpp>
using namespace tpl;
using namespace packets;

namespace tpl_tests
{
	TEST(field_descryptor_tests, can_get_first_field_of_simple_type)
	{
		byte_array bytes = {0x00, 0x01, 0x02, 0x03};
		using first_field = field_descriptor<uint32_t>;
		ASSERT_EQ(uint32_t(0x00010203), first_field::get(bytes));
	}

	TEST(field_descryptor_tests, can_get_non_first_field_of_simple_type)
	{
		byte_array bytes = {0x00, 0x01, 0x02, 0x03};
		using first_field = field_descriptor<uint8_t>;
		using second_field = field_descriptor<uint16_t, first_field>;
		ASSERT_EQ(uint16_t(0x0102), second_field::get(bytes));
	}

	TEST(field_descryptor_tests, can_get_byte_sequence_at_0_position)
	{
		byte_array bytes = {0x00, 0x01, 0x02, 0x03};
		using first_field = field_descriptor<details::byte_sequence<3>>;
		auto t = first_field::get(bytes);
		ASSERT_EQ(size_t(3), t.size());
		ASSERT_EQ(uint8_t(0x00), t[0]);
		ASSERT_EQ(uint8_t(0x01), t[1]);
		ASSERT_EQ(uint8_t(0x02), t[2]);
	}

	TEST(field_descryptor_tests, can_get_byte_sequence_after_some_fields)
	{
		byte_array bytes = {0x00, 0x01, 0x02, 0x03};
		using first_field = field_descriptor<uint16_t>;
		using second_field = field_descriptor<details::byte_sequence<2>, first_field>;
		auto t = second_field::get(bytes);
		ASSERT_EQ(size_t(2), t.size());
		ASSERT_EQ(uint8_t(0x02), t[0]);
		ASSERT_EQ(uint8_t(0x03), t[1]);
	}

	TEST(field_descryptor_tests, can_set_uint8t_field)
	{
		byte_array b{0x01, 0x02, 0x03};
		using field_desc = field_descriptor<uint8_t>;
		field_desc::set(b, 0x04);

		ASSERT_EQ(uint8_t(0x04), field_desc::get(b));
	}

	TEST(field_descryptor_tests, can_set_uint16t_field)
	{
		byte_array b{0x01, 0x02, 0x03};
		using field_desc = field_descriptor<uint16_t>;
		field_desc::set(b, 0x0405);

		ASSERT_EQ(uint16_t(0x0405), field_desc::get(b));
	}

	TEST(field_descryptor_tests, can_set_uint32t_field)
	{
		byte_array b{0x00, 0x00, 0x00, 0x00};
		using field_desc = field_descriptor<uint32_t>;
		field_desc::set(b, 0x01020304);

		ASSERT_EQ(uint32_t(0x01020304), field_desc::get(b));
	}

	TEST(field_descryptor_tests, can_set_uint64t_field)
	{
		byte_array b{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		using field_desc = field_descriptor<uint64_t>;
		field_desc::set(b, 0x0102030405060708);

		ASSERT_EQ(uint64_t(0x0102030405060708), field_desc::get(b));
	}

	TEST(field_descryptor_tests, can_set_byte_sequence_field)
	{
		byte_array b{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		using field_desc = field_descriptor<details::byte_sequence<5>>;

		auto value = byte_array{0x01, 0x02, 0x03, 0x04, 0x05};
		field_desc::set(b, value);

		auto get_back = field_desc::get(b);

		ASSERT_EQ(uint8_t(0x01), get_back[0]);
		ASSERT_EQ(uint8_t(0x02), get_back[1]);
		ASSERT_EQ(uint8_t(0x03), get_back[2]);
		ASSERT_EQ(uint8_t(0x04), get_back[3]);
		ASSERT_EQ(uint8_t(0x05), get_back[4]);
	}

	TEST(field_descryptor_tests, throws_invalid_argument_if_byte_sequence_length_mismatch_on_set)
	{
		byte_array b{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		using field_desc = field_descriptor<details::byte_sequence<5>>;

		auto value = byte_array{ 0x01, 0x02, 0x03, 0x04 };
		ASSERT_THROW(field_desc::set(b, value), std::out_of_range);
	}

	TEST(field_descryptor_tests, throws_out_of_range_if_dst_overflow)
	{
		auto t = []()
			{
				byte_array b{0x00, 0x00, 0x00, 0x00};
				using field_desc = field_descriptor<details::byte_sequence<5>>;

				auto value = byte_array{0x01, 0x02, 0x03, 0x04, 0x05};
				field_desc::set(b, value);
			};
		ASSERT_THROW(t(), std::out_of_range);
	}

	TEST(field_descryptor_tests, can_set_non_first_field)
	{
		byte_array b(100);
		for (auto& byte : b) byte = 0;

		using first_field = field_descriptor<uint16_t>;
		using second_field = field_descriptor<uint64_t, first_field>;
		using third_field = field_descriptor<uint32_t, second_field>;

		third_field::set(b, 0xdeadbeaf);

		ASSERT_EQ(uint32_t(0xdeadbeaf), third_field::get(b));
	}

	TEST(field_descryptor_tests, can_get_little_endian_field)
	{
		byte_array b({0x40,0x01});

		using f = field_descriptor<details::little_endian<uint16_t>>;
		ASSERT_EQ(uint16_t(0x0140), f::get(b));
	}

	TEST(field_descryptor_tests, little_endian_field_length_is_same_as_value_type)
	{
		ASSERT_EQ(sizeof(uint8_t), field_descriptor < details::little_endian<uint8_t> >::size);
		ASSERT_EQ(sizeof(uint16_t), field_descriptor < details::little_endian<uint16_t> >::size);
		ASSERT_EQ(sizeof(uint16_t), field_descriptor < details::little_endian<uint16_t> >::size);
		ASSERT_EQ(sizeof(uint64_t), field_descriptor < details::little_endian<uint64_t> >::size);
	}
}
