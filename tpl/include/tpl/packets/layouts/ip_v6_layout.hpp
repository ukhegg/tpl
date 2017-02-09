//
// Created by ukheg on 01.10.2016.
//

#ifndef PLAIN_PACKETS_IP_V6_LAYOUT_HPP
#define PLAIN_PACKETS_IP_V6_LAYOUT_HPP

#include "tpl/packets/layouts/ip_packet_layout.hpp"
#include "tpl/packets/ip_v6_packet.hpp"

namespace tpl{
    namespace packets{
        namespace layouts{
            class ip_v6_layout : public ip_packet_layout{
            public:
                using fields = ip_v6_packet::fields;

                static const std::shared_ptr<ip_v6_layout> instance;

                virtual byte_array header_bytes(const byte_array &bytes) const override;

                virtual byte_array payload_bytes(const byte_array &bytes) const override;

                virtual ip_version version(byte_array const &bytes) const override;

                virtual boost::asio::ip::address source_address(byte_array const &bytes) const override;

                virtual boost::asio::ip::address destination_address(byte_array const &bytes) const override;

                ip_payload_protocols next_header(byte_array const &bytes) const override;

                boost::asio::ip::address_v6 source(byte_array const &bytes) const;

                boost::asio::ip::address_v6 destination(byte_array const &bytes) const;

            protected:

	            object_id const& get_packet_id() const override;
            };
        }
    }
}



#endif //PLAIN_PACKETS_IP_V6_LAYOUT_HPP
