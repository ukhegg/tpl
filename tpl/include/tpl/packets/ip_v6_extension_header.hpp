//
// Created by ukheg on 02.10.2016.
//

#ifndef PLAIN_PACKETS_IP_V6_EXTENSION_HEADER_HPP
#define PLAIN_PACKETS_IP_V6_EXTENSION_HEADER_HPP

#include "tpl/packets/ip_packet.hpp"

namespace tpl {
    namespace packets {
        namespace layouts {
            class ip_v6_extension_header_layout;
        }

        class TPL_API ip_v6_extension_header : public packet {
            HAS_LAYOUT_OF_TYPE(layouts::ip_v6_extension_header_layout);
        public:
            struct fields {
                using next_header = field_descriptor<ip_payload_protocols>;

                static const size_t header_length = next_header::position + next_header::size;
            };

            ip_v6_extension_header();

            ip_v6_extension_header(byte_array const &bytes, ip_payload_protocols type);

            size_t total_length() const;

            ip_payload_protocols next_header() const;

            static bool is_extension_header_type(ip_payload_protocols protocol);

            static size_t next_header_size(byte_array const& bytes, ip_payload_protocols header_type);
        private:
            ip_payload_protocols type_;
        };
    }
}
REGISTER_TPL_OBJECT(tpl::packets::ip_v6_extension_header, tpl::packets::packet)

#endif //PLAIN_PACKETS_IP_V6_EXTENSION_HEADER_HPP
