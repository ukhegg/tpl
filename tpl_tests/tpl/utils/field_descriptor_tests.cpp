//
// Created by ukheg on 27.09.2016.
//
#include <gtest/gtest.h>
#include <tpl/packets/field_descriptor.hpp>

using namespace tpl;
using namespace packets;
using namespace details;

namespace tpl_tests {
    namespace utils {
        TEST(field_descriptor_tests, get_arithmetic_fields_getter_properly) {
            ASSERT_TRUE((std::is_same<field_descriptor<uint8_t>::getter_type, integer_getter<uint8_t, 0>>::value));
            ASSERT_TRUE((std::is_same<field_descriptor<uint16_t>::getter_type, integer_getter<uint16_t, 0>>::value));
            ASSERT_TRUE((std::is_same<field_descriptor<uint32_t>::getter_type, integer_getter<uint32_t, 0>>::value));
            ASSERT_TRUE((std::is_same<field_descriptor<uint64_t>::getter_type, integer_getter<uint64_t, 0>>::value));
            ASSERT_TRUE((std::is_same<field_descriptor<int8_t>::getter_type, integer_getter<int8_t, 0>>::value));
            ASSERT_TRUE((std::is_same<field_descriptor<int16_t>::getter_type, integer_getter<int16_t, 0>>::value));
            ASSERT_TRUE((std::is_same<field_descriptor<int32_t>::getter_type, integer_getter<int32_t, 0>>::value));
            ASSERT_TRUE((std::is_same<field_descriptor<int64_t>::getter_type, integer_getter<int64_t, 0>>::value));
        }

        TEST(field_descriptor_tests, get_enum_field_getter_properly) {
            enum enum1 {
                value
            };
            enum class enum2 {
                value
            };

            ASSERT_TRUE((std::is_same<field_descriptor<enum1>::getter_type, enum_getter<enum1, 0>>::value));
            ASSERT_TRUE((std::is_same<field_descriptor<enum2>::getter_type, enum_getter<enum2, 0>>::value));
        }

        TEST(field_descriptor_tests, get_array_field_getter_properly) {
            ASSERT_TRUE((std::is_same<field_descriptor<byte_sequence<5>>::getter_type, byte_sequence_getter<5, 0>>::value));
            ASSERT_TRUE((std::is_same<field_descriptor<byte_sequence<2>>::getter_type, byte_sequence_getter<2, 0>>::value));
        }

        TEST(field_descriptor_tests, arithmetic_getter_can_get_value_properly) {
            auto bytes = byte_array({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07});
            ASSERT_EQ(uint8_t(0x00), (integer_getter<uint8_t, 0>::get(bytes)));

            ASSERT_EQ(uint16_t(0x0100), (integer_getter<little_endian<uint16_t>, 0>::get(bytes)));
            ASSERT_EQ(uint16_t(0x0001), (integer_getter<big_endian<uint16_t>, 0>::get(bytes)));

            ASSERT_EQ(uint32_t(0x03020100), (integer_getter<little_endian<uint32_t>, 0>::get(bytes)));
            ASSERT_EQ(uint32_t(0x00010203), (integer_getter<big_endian<uint32_t>, 0>::get(bytes)));

            ASSERT_EQ(uint64_t(0x0706050403020100), (integer_getter<little_endian<uint64_t>, 0>::get(bytes)));
            ASSERT_EQ(uint64_t(0x0001020304050607), (integer_getter<big_endian<uint64_t>, 0>::get(bytes)));
        }

        TEST(field_descriptor_tests, arithmetic_getter_works_properly_at_non_zero_offset) {
            auto bytes = byte_array({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07});

            ASSERT_EQ(uint16_t(0x0102), (integer_getter<uint16_t, 1>::get(bytes)));
            ASSERT_EQ(uint16_t(0x0102), (integer_getter<big_endian<uint16_t>, 1>::get(bytes)));
        }

        TEST(field_descriptor_tests, arithmetic_getter_throws_exception_if_out_of_bounds) {
            auto bytes = byte_array({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07});

            ASSERT_THROW((integer_getter<uint8_t, 8>::get(bytes)), std::invalid_argument);

            ASSERT_THROW((integer_getter<uint16_t, 7>::get(bytes)), std::invalid_argument);
            ASSERT_THROW((integer_getter<uint16_t, 8>::get(bytes)), std::invalid_argument);
        }

        TEST(field_descriptor_tests, enum_getter_works_properly) {
            auto bytes = byte_array({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07});
            enum class enum1 : uint8_t{
                val0 = 0x00,
                val1 = 0x01,
                val2 = 0x02
            };

            enum class enum2 : uint16_t{
                val0 = 0x0001,
                val1 = 0x0102,
                val2 = 0x0203
            };

            ASSERT_EQ(enum1::val0, (enum_getter<enum1, 0>::get(bytes)));
            ASSERT_EQ(enum1::val2, (enum_getter<enum1, 2>::get(bytes)));
            ASSERT_EQ(enum1::val1, (enum_getter<enum1, 1>::get(bytes)));

            ASSERT_EQ(enum2::val0, (enum_getter<enum2, 0>::get(bytes)));
            ASSERT_EQ(enum2::val1, (enum_getter<enum2, 1>::get(bytes)));
            ASSERT_EQ(enum2::val2, (enum_getter<enum2, 2>::get(bytes)));
        }

        TEST(field_descriptor_tests, array_getter_works_properly){
            auto bytes = byte_array({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07});
            using getter_type =  byte_sequence_getter<6, 1>;

            auto t = getter_type::get(bytes);
            ASSERT_EQ(size_t(6), t.size());
            ASSERT_EQ(uint8_t(0x01), t[0]);
            ASSERT_EQ(uint8_t(0x02), t[1]);
            ASSERT_EQ(uint8_t(0x03), t[2]);
            ASSERT_EQ(uint8_t(0x04), t[3]);
            ASSERT_EQ(uint8_t(0x05), t[4]);
            ASSERT_EQ(uint8_t(0x06), t[5]);
        }

        TEST(field_descriptor_tests, get_arithmetic_fields_setter_properly) {
            ASSERT_TRUE((std::is_same<field_descriptor<uint8_t>::setter_type , integer_setter<uint8_t, 0>>::value));
            ASSERT_TRUE((std::is_same<field_descriptor<uint16_t>::setter_type, integer_setter<uint16_t, 0>>::value));
            ASSERT_TRUE((std::is_same<field_descriptor<uint32_t>::setter_type, integer_setter<uint32_t, 0>>::value));
            ASSERT_TRUE((std::is_same<field_descriptor<uint64_t>::setter_type, integer_setter<uint64_t, 0>>::value));
            ASSERT_TRUE((std::is_same<field_descriptor<int8_t>::setter_type, integer_setter<int8_t, 0>>::value));
            ASSERT_TRUE((std::is_same<field_descriptor<int16_t>::setter_type, integer_setter<int16_t, 0>>::value));
            ASSERT_TRUE((std::is_same<field_descriptor<int32_t>::setter_type, integer_setter<int32_t, 0>>::value));
            ASSERT_TRUE((std::is_same<field_descriptor<int64_t>::setter_type, integer_setter<int64_t, 0>>::value));
        }

        TEST(field_descriptor_tests, get_enum_field_setter_properly) {
            enum enum1 {
                value
            };
            enum class enum2 {
                value
            };

            ASSERT_TRUE((std::is_same<field_descriptor<enum1>::setter_type, enum_setter<enum1, 0>>::value));
            ASSERT_TRUE((std::is_same<field_descriptor<enum2>::setter_type, enum_setter<enum2, 0>>::value));
        }

        TEST(field_descriptor_tests, get_array_field_setter_properly) {
            ASSERT_TRUE((std::is_same<field_descriptor<byte_sequence<5>>::setter_type, byte_sequence_setter<5, 0>>::value));
            ASSERT_TRUE((std::is_same<field_descriptor<byte_sequence<2>>::setter_type, byte_sequence_setter<2, 0>>::value));
        }

        TEST(field_descriptor_tests, arithmetic_setter_can_get_value_properly) {
            auto bytes = byte_array({0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00});

            integer_setter<uint8_t, 0>::set(bytes, 0x01);
            ASSERT_EQ(uint8_t(0x01), (integer_getter<uint8_t, 0>::get(bytes)));

            integer_setter<uint16_t, 0>::set(bytes, 0x0102);
            ASSERT_EQ(uint16_t(0x0102), (integer_getter<uint16_t, 0>::get(bytes)));

            integer_setter<uint32_t, 0>::set(bytes, 0x01020304);
            ASSERT_EQ(uint32_t(0x01020304), (integer_getter<uint32_t, 0>::get(bytes)));

            integer_setter<uint64_t, 0>::set(bytes, 0x01020304050607);
            ASSERT_EQ(uint64_t(0x01020304050607), (integer_getter<uint64_t, 0>::get(bytes)));
        }

        TEST(field_descriptor_tests, enum_setter_works_properly) {
            auto bytes = byte_array({0x00, 0x00,0x00,0x00});
            enum class enum1 : uint8_t{
                val0 = 0x00,
                val1 = 0x01,
                val2 = 0x02
            };

            enum class enum2 : uint16_t{
                val0 = 0x0100,
                val1 = 0x0201,
                val2 = 0x0302
            };

            using setter_type1 = enum_setter<enum1, 0>;
            setter_type1::set(bytes, enum1::val1);
            using getter_type1 = enum_getter<enum1, 0>;
            ASSERT_EQ(enum1::val1, getter_type1::get(bytes));

            using setter_type2 = enum_setter<enum2, 0>;
            setter_type2::set(bytes, enum2::val1);
            using getter_type2 = enum_getter<enum2, 0>;
            ASSERT_EQ(enum2::val1, getter_type2::get(bytes));
        }

        TEST(field_descriptor_tests, integer_field_without_previous_has_valid_params)
        {
            ASSERT_EQ(0, field_descriptor<uint32_t>::position);
            ASSERT_EQ(4, field_descriptor<uint32_t>::size);
        }

        TEST(field_descriptor_tests, integer_field_with_some_previous_has_valid_params)
        {
            using f1 = field_descriptor<uint8_t>;
            using f2 = field_descriptor<uint32_t, f1>;

            ASSERT_EQ(1, f2::position);
            ASSERT_EQ(4, f2::size);
        }

        TEST(field_descriptor_tests, byte_sequence_field_has_valid_params)
        {
            using f1 = field_descriptor<byte_sequence<5>>;
            using f2 = field_descriptor<byte_sequence<3>, f1>;

            ASSERT_EQ(5, f1::size);
            ASSERT_EQ(0, f1::position);

            ASSERT_EQ(3, f2::size);
            ASSERT_EQ(5, f2::position);
        }

        TEST(field_descriptor_tests, enum_field_gets_right_parameters){
            enum class enum1 : uint8_t{
                val0 = 0x00,
                val1 = 0x01,
                val2 = 0x02
            };

            ASSERT_EQ(1, field_descriptor<enum1>::size);
            ASSERT_EQ(0, field_descriptor<enum1>::position);

            enum class enum2 : uint16_t{
                val0 = 0x0001,
                val1 = 0x0002,
                val2 = 0x0003
            };

            ASSERT_EQ(2, field_descriptor<enum2>::size);
            ASSERT_EQ(0, field_descriptor<enum2>::position);
        }

        TEST(field_descriptor_tests, big_little_endian_integer_field_gets_right_size){

            ASSERT_EQ(1, field_descriptor<big_endian<uint8_t>>::size);
            ASSERT_EQ(2, field_descriptor<big_endian<uint16_t>>::size);
            ASSERT_EQ(4, field_descriptor<big_endian<uint32_t>>::size);
            ASSERT_EQ(8, field_descriptor<big_endian<uint64_t>>::size);

            ASSERT_EQ(1, field_descriptor<little_endian<uint8_t>>::size);
            ASSERT_EQ(2, field_descriptor<little_endian<uint16_t>>::size);
            ASSERT_EQ(4, field_descriptor<little_endian<uint32_t>>::size);
            ASSERT_EQ(8, field_descriptor<little_endian<uint64_t>>::size);
        }
    }
}
