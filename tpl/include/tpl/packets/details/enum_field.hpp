//
// Created by ukheg on 28.09.2016.
//

#ifndef PLAIN_PACKETS_ENUM_FIELD_HPP
#define PLAIN_PACKETS_ENUM_FIELD_HPP

#include "tpl/utils/d_array.hpp"
#include "integer_field.hpp"
#include <boost/endian/conversion.hpp>

namespace tpl {
    namespace packets {
        namespace details {
            template<class t_enum, size_t t_offset>
            struct enum_getter {
                static t_enum get(byte_array const &bytes) {
                    using underlying_int_type = big_endian<typename std::underlying_type<t_enum>::type>;
                    using value_getter_type = integer_getter<underlying_int_type, t_offset>;
                    return (t_enum) value_getter_type::get(bytes);
                }
            };

            template<class t_enum, size_t t_offset>
            struct enum_getter<little_endian < t_enum>, t_offset> {
                static t_enum get(byte_array const &bytes) {
                    using underlying_int_type = little_endian<typename std::underlying_type<t_enum>::type>;
                    using value_getter_type = integer_getter<underlying_int_type, t_offset>;
                    return (t_enum) value_getter_type::get(bytes);
                }
            };

            template<class t_enum, size_t t_offset>
            struct enum_getter<big_endian < t_enum>, t_offset> {
                static t_enum get(byte_array const &bytes) {
                    using underlying_int_type = big_endian<typename std::underlying_type<t_enum>::type>;
                    using value_getter_type = integer_getter<underlying_int_type, t_offset>;
                    return (t_enum) value_getter_type::get(bytes);
                }
            };

            template<class t_enum, size_t t_offset>
            struct enum_setter {
                static void set(byte_array &bytes, t_enum value) {
                    using underlying_int_type = typename std::underlying_type<t_enum>::type;
                    using value_setter_type = integer_setter <big_endian<underlying_int_type>, t_offset>;
                    value_setter_type::set(bytes, (underlying_int_type) value);
                }
            };

            template<class t_enum, size_t t_offset>
            struct enum_setter<little_endian < t_enum>, t_offset> {
                static void set(byte_array &bytes, t_enum value) {
                    using underlying_int_type = typename std::underlying_type<t_enum>::type;
                    using value_setter_type = integer_setter <little_endian<underlying_int_type>, t_offset>;
                    value_setter_type::set(bytes, (underlying_int_type) value);
                }
            };

            template<class t_enum, size_t t_offset>
            struct enum_setter<big_endian < t_enum>, t_offset> {
                static void set(byte_array &bytes, t_enum value) {
                    using underlying_int_type = typename std::underlying_type<t_enum>::type;
                    using value_setter_type = integer_setter <big_endian<underlying_int_type>, t_offset>;
                    value_setter_type::set(bytes, t_offset, (underlying_int_type) value);
                }
            };

            template<class t_field_type>
            struct is_enum_field {
                static const auto value = std::is_enum<t_field_type>::value;
            };

            template<class t_field_type>
            struct is_enum_field<little_endian < t_field_type>> {
                static const auto value = std::is_enum<t_field_type>::value;
            };

            template<class t_field_type>
            struct is_enum_field<big_endian < t_field_type>> {
                static const auto value = std::is_enum<t_field_type>::value;
            };
        }
    }
}
#endif //PLAIN_PACKETS_ENUM_FIELD_HPP
