#include "tpl/packets/transport/tcp_option.hpp"


namespace tpl {
    namespace packets {
        tcp_option::tcp_option(const byte_array &bytes)
                : bytes_(bytes) {}


        tcp_option::~tcp_option() {}

        tcp_option::option_types tcp_option::type() const {
            return option_types(this->bytes_[0]);
        }

        uint8_t tcp_option::length() const {
            return option_size(this->bytes_);
        }

        uint8_t tcp_option::option_size(byte_array const& bytes) {
            switch (bytes[0]) {
                case static_cast<uint8_t>(option_types::no_operation):
                case static_cast<uint8_t>(option_types::end_of_options):
                    return 1;
                default:
                    return bytes[1];
            }
        }

        const tpl::utils::d_array<unsigned char> &tpl::packets::tcp_option::bytes() const {
            return this->bytes_;
        }
    }
}

