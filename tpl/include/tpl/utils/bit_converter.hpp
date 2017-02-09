#pragma once

#include "tpl/utils/d_array.hpp"
#include <boost/endian/buffers.hpp>
#include <stdint.h>
#include <stdexcept>
namespace tpl {
    namespace utils {
        template<class t_result>
        t_result ntoh_unsafe(const uint8_t *buffer, size_t offset) {
            static_assert(std::is_arithmetic<t_result>::value, "t_result must be of arithmetic type");
            return boost::endian::big_to_native(reinterpret_cast<const t_result *>(buffer + offset)[0]);
        }

        template<class t_result>
        t_result ltoh_unsafe(const uint8_t *buffer, size_t offset) {
            static_assert(std::is_arithmetic<t_result>::value, "t_result must be of arithmetic type");
            return boost::endian::little_to_native(reinterpret_cast<const t_result *>(buffer + offset)[0]);
        }

        template<class t_result>
        t_result ntoh_safe(const uint8_t *buffer, size_t buffer_size, size_t offset) {
            if (buffer_size < offset + sizeof(t_result)) {
                throw std::out_of_range("ntoh condition failure: buffer_size >= offset + sizeof(t_result) ");
            }
            return ntoh_unsafe<t_result>(buffer, offset);
        }

        template<class t_result>
        t_result ltoh_safe(const uint8_t *buffer, size_t buffer_size, size_t offset) {
            if (buffer_size < offset + sizeof(t_result)) {
                throw std::out_of_range("ntoh condition failure: buffer_size >= offset + sizeof(t_result) ");
            }
            return ltoh_unsafe<t_result>(buffer, offset);
        }

        template<class t_result>
        t_result ntoh(byte_array const &arr, size_t pos) {
            return ntoh_safe<t_result>(arr.data(), arr.size(), pos);
        }

        template<class t_result>
        t_result ltoh(byte_array const &arr, size_t pos) {
            return ltoh_safe<t_result>(arr.data(), arr.size(), pos);
        }

        template<class t_result>
        void hton_unsafe(uint8_t *result, size_t offset, t_result value) {
            static_assert(std::is_arithmetic<t_result>::value, "t_result must be of arithmetic type");
            reinterpret_cast<t_result *>(result + offset)[0] = boost::endian::native_to_big(value);
        }

        template<class t_result>
        void hton_safe(uint8_t *buffer, size_t buffer_size, size_t offset, t_result value) {
            if (buffer_size < offset + sizeof(t_result)) {
                throw std::out_of_range("hton_safe condition failure: buffer_size >= offset + sizeof(t_result)");
            }
            hton_unsafe(buffer, offset, value);
        }

        template<class t_result>
        void hton(byte_array &arr, size_t pos, t_result n) {
            hton_safe(&arr[0], arr.size(), pos, n);
        }
    }
}

