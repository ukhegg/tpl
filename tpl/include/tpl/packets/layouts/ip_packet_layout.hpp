//
// Created by ukheg on 01.10.2016.
//

#ifndef PLAIN_PACKETS_IP_PACKET_LAYOUT_HPP
#define PLAIN_PACKETS_IP_PACKET_LAYOUT_HPP
#include "tpl/packets/ip_packet.hpp"

namespace tpl{
    namespace packets{
        namespace layouts{
            class ip_packet_layout : public fields_layout{
            public:
                using fields = ip_packet::fields;

                virtual ip_version version(byte_array const& bytes) const;

                virtual boost::asio::ip::address source_address(byte_array const& bytes) const = 0;

                virtual boost::asio::ip::address destination_address(byte_array const& bytes) const = 0;

                virtual ip_payload_protocols next_header(byte_array const &bytes) const = 0;

            protected:

	            object_id const& get_packet_id() const override;
            };
        }
    }
}
#endif //PLAIN_PACKETS_IP_PACKET_LAYOUT_HPP
