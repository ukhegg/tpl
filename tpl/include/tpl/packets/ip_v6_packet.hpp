//
// Created by ukheg on 01.10.2016.
//

#ifndef PLAIN_PACKETS_IP_V6_PACKET_HPP
#define PLAIN_PACKETS_IP_V6_PACKET_HPP

#include "tpl/packets/ip_packet.hpp"
#include "tpl/packets/ip_v6_extension_headers_sequence.hpp"
#include "tpl/packets/data_link/ethernet_packet.hpp"

namespace tpl{
    namespace packets{
        namespace layouts{class ip_v6_layout;}

        class TPL_API ip_v6_packet : public ip_packet{
            HAS_LAYOUT_OF_TYPE(layouts::ip_v6_layout)
        public:
            struct version_traffic_class_and_flow_label_tuple {
                uint8_t version : 4;
                uint8_t traffic_class : 8;
                uint32_t flow_label : 20;
            };

            struct fields {
                using version_traffic_and_flow_label = field_descriptor<uint32_t>;
                using payload_length = field_descriptor<uint16_t, version_traffic_and_flow_label>;
                using next_header = field_descriptor<ip_payload_protocols , payload_length>;
                using hop_limit = field_descriptor<uint8_t, next_header>;
                using source_addr = field_descriptor<details::byte_sequence<16>, hop_limit>;
                using destination_addr = field_descriptor<details::byte_sequence<16>, source_addr>;

                static const int header_length = destination_addr::position + destination_addr::size;
                static const int extension_headers_position = header_length;
            };

            explicit ip_v6_packet(const byte_array &bytes);

            ip_v6_packet(const byte_array &bytes, layout_ptr const& layout);

			explicit ip_v6_packet(placeholders::parent_packet_holder<ip_packet> const& parent);
			
			explicit ip_v6_packet(placeholders::parent_packet_holder<ethernet_packet> const& parent);

            uint8_t traffic_class() const;

            uint32_t flow_label() const;

            uint16_t payload_length() const;

            uint8_t hop_limit() const;

            boost::asio::ip::address_v6 source() const;

            boost::asio::ip::address_v6 destination() const;

            ip_v6_extension_headers_sequence const& extension_headers() const;
        private:
            mutable boost::optional<ip_v6_extension_headers_sequence> extension_headers_;

            ip_v6_extension_headers_sequence read_extension_headers() const;
        };
		
		PACKET_CAST_DECLARATION(ip_v6_packet);
    }
}
REGISTER_TPL_OBJECT(tpl::packets::ip_v6_packet, tpl::packets::ip_packet);


#endif //PLAIN_PACKETS_IP_V6_PACKET_HPP
