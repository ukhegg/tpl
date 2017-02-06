#pragma once

#include "tpl/tpl.hpp"
#include "d_array.hpp"
#include <array>

namespace tpl {
    namespace utils {
        class TPL_API mac_address {
        public:
            static const size_t mac_address_length = 6;
            static const uint8_t docsis_multicast[5];
            static const mac_address multycast;
            typedef std::array<uint8_t, mac_address_length> bytes_type;

            explicit mac_address();

            explicit mac_address(const uint8_t *bytes);

            explicit mac_address(byte_array const &bytes);

            explicit mac_address(bytes_type const &bytes);

            inline bool operator==(mac_address const &rhv) const {
                return memcmp(bytes_.data(), rhv.bytes_.data(), mac_address_length) == 0;
            }

            inline bool operator!=(mac_address const &rhv) const {
                return 0 != memcmp(bytes_.data(), rhv.bytes_.data(), mac_address_length);
            }

            inline unsigned char operator[](int index) const {
                return bytes_[index];
            }

            inline unsigned char &operator[](int index) {
                return bytes_[index];
            }

            const inline bytes_type &bytes() const {
                return this->bytes_;
            }

            template<class t_char>
            inline std::basic_string<t_char> to_string() const;

        private:
            bytes_type bytes_;

            static const size_t printable_string_length = mac_address_length * 3;

            static mac_address get_multicast();
        };

        template<class t_char>
        std::basic_string<t_char> mac_address::to_string() const {
            throw std::runtime_error("not implemented");
        }

        template<>
        inline std::basic_string<char> mac_address::to_string() const {
            char buffer[printable_string_length];
            sprintf_s(buffer, "%02x:%02x:%02x:%02x:%02x:%02x", bytes_[0], bytes_[1], bytes_[2], bytes_[3], bytes_[4],
                      bytes_[5]);
            return std::string(buffer);
        }

        template<>
        inline std::basic_string<wchar_t> mac_address::to_string() const {
            wchar_t buffer[printable_string_length];
            swprintf_s(buffer, L"%02x:%02x:%02x:%02x:%02x:%02x", bytes_[0], bytes_[1], bytes_[2], bytes_[3], bytes_[4],
                       bytes_[5]);
            return std::wstring(buffer);
        }
    }

}

namespace std {
    template<>
    struct hash<tpl::utils::mac_address> : public unary_function<tpl::utils::mac_address, size_t> {
        size_t operator()(tpl::utils::mac_address const &addr) const {
            return reinterpret_cast<const unsigned int *>(addr.bytes().data())[0];
        }
    };
}

