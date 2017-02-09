//
// Created by ukheg on 05.10.2016.
//

#ifndef PLAIN_PACKETS_UDP_PACKET_HPP
#define PLAIN_PACKETS_UDP_PACKET_HPP

#include "tpl/packets/ip_packet.hpp"
#include "tpl/packets/transport/transport_packet.hpp"
#include <tpl/packets/ip_v4_packet.hpp>
#include <tpl/packets/ip_v6_packet.hpp>

namespace tpl {
    namespace packets {
        namespace layouts { class udp_packet_layout; }

        class TPL_API udp_packet : public transport_packet {
            HAS_LAYOUT_OF_TYPE(layouts::udp_packet_layout)
        public:
            struct fields {
                using source_port = field_descriptor<uint16_t>;
                using destination_port = field_descriptor<uint16_t, source_port>;
                using length = field_descriptor<uint16_t, destination_port>;
                using checksum = field_descriptor<uint16_t, length>;

                static const size_t header_length = checksum::position + checksum::size;
            };

	        udp_packet();

	        explicit udp_packet(byte_array const &bytes);
	        
			udp_packet(byte_array const &bytes, layout_ptr const& layout);

	        explicit udp_packet(placeholders::parent_packet_holder<ip_packet> parent);

	        static boost::optional<udp_packet> try_create(placeholders::parent_packet_holder<ip_packet> parent);

	        static boost::optional<udp_packet> try_create(byte_array const& bytes);

            uint16_t length() const;

            uint16_t checksum() const;

			static uint16_t calculate_checksum(const udp_packet &packet,
				boost::asio::ip::address_v4 const &src,
				boost::asio::ip::address_v4 const &dst);
        };

		PACKET_CAST_DECLARATION(udp_packet);
    }
}
REGISTER_TPL_OBJECT(tpl::packets::udp_packet, tpl::packets::transport_packet);

#endif //PLAIN_PACKETS_UDP_PACKET_HPP
