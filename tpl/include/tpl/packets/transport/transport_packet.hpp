//
// Created by ukheg on 05.10.2016.
//

#ifndef PLAIN_PACKETS_TRANSPORT_PACKET_HPP
#define PLAIN_PACKETS_TRANSPORT_PACKET_HPP

#include "tpl/packets/packet.hpp"
#include "tpl/packets/transport/transport_type.hpp"
#include "transport_ports.hpp"

namespace tpl{
    namespace packets{
        namespace layouts{ class transport_packet_layout; }

        class TPL_API transport_packet : public packet{
            HAS_LAYOUT_OF_TYPE(layouts::transport_packet_layout)
        public:
            transport_packet();

			transport_packet(const byte_array &bytes, std::shared_ptr<layouts::transport_packet_layout> const& layout);

            transport_type type() const;

            uint16_t src_port() const;

            uint16_t dst_port() const;

			transport_ports ports() const;
        };

		PACKET_CAST_DECLARATION(transport_packet);
    }
}

REGISTER_TPL_OBJECT(tpl::packets::transport_packet, tpl::packets::packet);

#endif //PLAIN_PACKETS_TRANSPORT_PACKET_HPP
