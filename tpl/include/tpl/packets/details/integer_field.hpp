//
// Created by ukheg on 28.09.2016.
//

#ifndef PLAIN_PACKETS_INTEGER_FIELD_HPP
#define PLAIN_PACKETS_INTEGER_FIELD_HPP

#include "packet_fields.hpp"
#include "tpl/utils/d_array.hpp"
#include <boost/endian/conversion.hpp>

namespace tpl {
    namespace packets {
        namespace details {
            struct bit_converter {
                template<class t_integer>
                static t_integer get(byte_array const &bytes, size_t offset){
                    if (bytes.size() < offset + sizeof(t_integer)) throw std::invalid_argument("out of bounds");
                    return boost::endian::big_to_native(
                            reinterpret_cast<const t_integer *>(bytes.data() + offset)[0]);
                }
            };

            template<class t_integer, size_t t_offset>
            struct integer_getter {
                static t_integer get(byte_array const &bytes) {
                    if (bytes.size() < t_offset + sizeof(t_integer)) throw std::invalid_argument("out of bounds");
                    return boost::endian::big_to_native(
                            reinterpret_cast<const t_integer *>(bytes.data() + t_offset)[0]);
                }
            };

            template<class t_integer, size_t t_offset>
            struct integer_getter<little_endian<t_integer>, t_offset> {
                static t_integer get(byte_array const &bytes) {
                    if (bytes.size() < t_offset + sizeof(t_integer)) throw std::invalid_argument("not enough bytes");
                    return boost::endian::little_to_native(
                            reinterpret_cast<const t_integer *>(bytes.data() + t_offset)[0]);
                }
            };

            template<class t_integer, size_t t_offset>
            struct integer_getter<big_endian<t_integer>, t_offset> {
                static t_integer get(byte_array const &bytes) {
                    if (bytes.size() < t_offset + sizeof(t_integer)) throw std::invalid_argument("not enough bytes");
                    return boost::endian::big_to_native(
                            reinterpret_cast<const t_integer *>(bytes.data() + t_offset)[0]);
                }
            };

            template<class t_integer, size_t t_offset>
            struct integer_setter {
                static void set(byte_array &bytes, t_integer value) {
                    if (bytes.size() < t_offset + sizeof(t_integer)) throw std::invalid_argument("out of bounds");
                    reinterpret_cast<t_integer *>(bytes.data() + t_offset)[0] = boost::endian::native_to_big(value);
                }
            };

            template<class t_integer, size_t t_offset>
            struct integer_setter<little_endian<t_integer>, t_offset> {
                static void set(byte_array &bytes, t_integer value) {
                    if (bytes.size() < t_offset + sizeof(t_integer)) throw std::invalid_argument("out of bounds");
                    reinterpret_cast<t_integer *>(bytes.data() + t_offset)[0] = boost::endian::native_to_little(value);
                }
            };

            template<class t_integer, size_t t_offset>
            struct integer_setter<big_endian<t_integer>, t_offset> {
                static void set(byte_array &bytes, t_integer value) {
                    if (bytes.size() < t_offset + sizeof(t_integer)) throw std::invalid_argument("out of bounds");
                    reinterpret_cast<t_integer *>(bytes.data() + t_offset)[0] = boost::endian::big_to_native(value);
                }
            };

            template<class t_field_type>
            struct is_integral_field {
                static const auto value = std::is_integral<t_field_type>::value;
            };

            template<class t_field_type>
            struct is_integral_field<little_endian<t_field_type>> {
                static const auto value = std::is_integral<t_field_type>::value;
            };

            template<class t_field_type>
            struct is_integral_field<big_endian<t_field_type>> {
                static const auto value = std::is_integral<t_field_type>::value;
            };
        }
    }
}
#endif //PLAIN_PACKETS_INTEGER_FIELD_HPP
