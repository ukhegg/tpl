//
// Created by ukheg on 05.10.2016.
//

#include "tpl/packets/transport/transport_packet.hpp"
#include "tpl/packets/layouts/transport_packet_layout.hpp"

namespace tpl {
    namespace packets {
        uint16_t transport_packet::src_port() const {
            return this->layout().src_port(this->bytes_);
        }

        uint16_t transport_packet::dst_port() const {
            return this->layout().dst_port(this->bytes_);
        }

	    transport_ports transport_packet::ports() const
        {
			return transport_ports(this->src_port(), this->dst_port(), this->type());
        }

	    transport_packet::transport_packet(const byte_array &bytes,
                                           std::shared_ptr<layouts::transport_packet_layout> const &layout)
                : packet(bytes, layout) {

        }

        transport_type transport_packet::type() const {
            return this->layout().type(this->bytes_);
        }

        transport_packet::transport_packet() {

        }
    }
}

