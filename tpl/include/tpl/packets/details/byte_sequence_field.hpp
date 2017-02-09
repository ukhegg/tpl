//
// Created by ukheg on 28.09.2016.
//

#ifndef PLAIN_PACKETS_BYTE_SEQUENCE_FIELD_HPP
#define PLAIN_PACKETS_BYTE_SEQUENCE_FIELD_HPP

#include "tpl/utils/d_array.hpp"

namespace tpl {
    namespace packets {
        namespace details {
            template<size_t t_sequence_length>
            struct byte_sequence {
                enum {
                    length = t_sequence_length
                };
            private:
                uint8_t dummy_[t_sequence_length] ;//for sizeof(byte_sequence<N>) == N
            };

            template<size_t t_sequence_length, size_t t_offset>
            struct byte_sequence_getter {
                static byte_array get(byte_array const &bytes) {
                    static_assert(t_sequence_length != 0, "array must be fixed(>0) size");
                    return bytes.middle(t_offset, t_sequence_length);
                }

                static std::array<uint8_t, t_sequence_length> get_array(byte_array const &bytes) {
                    static_assert(t_sequence_length != 0, "array must be fixed(>0) size");
                    std::array<uint8_t, t_sequence_length> res;
                    std::copy(bytes.begin() + t_offset,
                              bytes.begin() + t_offset + t_sequence_length,
                              res.begin());
                    return res;
                };
            };

            template<size_t t_sequence_length, size_t t_offset>
            struct byte_sequence_setter {
                static void set(byte_array &bytes, byte_array const &value) {
                    static_assert(t_sequence_length != 0, "array must be fixed(>0) size");
					if(value.size() != t_sequence_length)
					{
						throw std::out_of_range("invalid value array length");
					}
					if(bytes.size() < t_sequence_length + t_offset)
					{
						throw std::out_of_range("invalid dst array size");
					}
                    std::copy(value.begin(), value.begin() + t_sequence_length, bytes.begin() + t_offset);
                }

                static void set(byte_array &bytes, uint8_t *value) {
                    static_assert(t_sequence_length != 0, "array must be fixed(>0) size");
					if (bytes.size() < t_sequence_length + t_offset)
					{
						throw std::out_of_range("invalid dst array size");
					}
                    std::copy(value, value + t_sequence_length, bytes.begin() + t_offset);
                }

                static void set(byte_array &bytes, std::array<uint8_t, t_sequence_length> const& value) {
                    static_assert(t_sequence_length != 0, "array must be fixed(>0) size");
					if (bytes.size() < t_sequence_length + t_offset)
					{
						throw std::out_of_range("invalid dst array size");
					}
                    std::copy(value.begin(), value.end(), bytes.begin() + t_offset);
                }

                static void set(byte_array &bytes, std::initializer_list<uint8_t> const& value) {
                    static_assert(t_sequence_length != 0, "array must be fixed(>0) size");
                    if(value.size() != t_sequence_length) throw std::invalid_argument("invalid value param-invalid length");
					if (bytes.size() < t_sequence_length + t_offset)
					{
						throw std::out_of_range("invalid dst array size");
					}
                    std::copy(value.begin(), value.end(), bytes.begin() + t_offset);
                }
            };

            template<class t>
            struct is_byte_sequence {
                static const auto value = false;
            };

            template<size_t t_sequence_length>
            struct is_byte_sequence<byte_sequence<t_sequence_length>> {
                static const auto value = true;
            };
        }
    }
}
#endif //PLAIN_PACKETS_BYTE_SEQUENCE_FIELD_HPP
