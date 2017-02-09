//
// Created by ukheg on 01.10.2016.
//

#ifndef PLAIN_PACKETS_IP_V4_LAYOUT_HPP
#define PLAIN_PACKETS_IP_V4_LAYOUT_HPP
#include "tpl/packets/layouts/ip_packet_layout.hpp"
#include "tpl/packets/ip_v4_packet.hpp"


namespace tpl{
    namespace packets{
        namespace layouts{
            class ip_v4_layout : public ip_packet_layout {
            public:
                using fields = ip_v4_packet::fields;

                static const std::shared_ptr<ip_v4_layout> instance;

                byte_array header_bytes(const byte_array &bytes) const override;

                byte_array payload_bytes(const byte_array &bytes) const override;

                uint8_t header_length(const byte_array &bytes) const;

                uint16_t total_length(byte_array const& bytes) const;

                ip_payload_protocols next_header(byte_array const& bytes) const override;

                boost::asio::ip::address_v4 source(byte_array const& bytes) const;

                virtual boost::asio::ip::address source_address(byte_array const &bytes) const override;

                boost::asio::ip::address_v4 destination(byte_array const& bytes) const;

                virtual boost::asio::ip::address destination_address(byte_array const &bytes) const override;

            protected:

	            object_id const& get_packet_id() const override;
            };
        }
    }
}



#endif //PLAIN_PACKETS_IP_V4_LAYOUT_HPP
