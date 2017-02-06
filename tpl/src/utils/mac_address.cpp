#include "tpl/utils/mac_address.hpp"

namespace tpl {
    namespace utils {
        mac_address::mac_address() {}

        mac_address::mac_address(const uint8_t *bytes) {
            memcpy(this->bytes_.data(), bytes, mac_address_length);
        }

        mac_address::mac_address(byte_array const &bytes) {
            if (bytes.size() < mac_address_length) throw std::invalid_argument("invalid byte array length");

            memcpy(this->bytes_.data(), bytes.data(), mac_address_length);
        }

        mac_address::mac_address(bytes_type const &bytes)
                : bytes_(bytes) {}

        mac_address mac_address::get_multicast() {
            mac_address addr;
            for (auto &b : addr.bytes_) {
                b = 0xff;
            }
            return addr;
        }

        const mac_address mac_address::multycast = mac_address::get_multicast();
    }

}

