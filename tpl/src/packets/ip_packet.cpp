//
// Created by ukheg on 26.09.2016.
//

#include "tpl/packets/ip_packet.hpp"
#include "tpl/packets/layouts/ip_packet_layout.hpp"
#include "tpl/packets/layouts/ip_v4_layout.hpp"
#include "tpl/packets/layouts/ip_v6_layout.hpp"

namespace tpl {
    namespace packets {

        namespace {
            std::shared_ptr<layouts::ip_packet_layout> choose_layout(byte_array const &bytes) {
                switch (layouts::ip_v4_layout::instance->version(bytes)) {
                    case ip_version::ip_v4:
                        return layouts::ip_v4_layout::instance;
                    case ip_version::ip_v6:
                        return layouts::ip_v6_layout::instance;
                    default:
                        throw std::invalid_argument("can\'n determine ip version");
                }
            }
        }

        ip_packet::ip_packet() {

        }

        ip_packet::ip_packet(byte_array const &bytes)
                : packet(bytes, choose_layout(bytes)) {

        }

	    ip_packet::ip_packet(byte_array const& bytes, layout_ptr const& layout)
			: packet(bytes, layout){
	    }

	    ip_packet::ip_packet(placeholders::parent_packet_holder<packet> parent)
                : ip_packet(parent.packet.payload_bytes()) {
        }

        ip_version ip_packet::version() const {
            return this->layout().version(this->bytes_);
        }

        boost::asio::ip::address ip_packet::source_address() const {
            return this->layout().source_address(this->bytes_);
        }

        boost::asio::ip::address ip_packet::destination_address() const {
            return this->layout().destination_address(this->bytes_);
        }


        ip_payload_protocols ip_packet::next_header() const {
            return this->layout().next_header(this->bytes_);
        }
		
		PACKET_CAST_DEFINITION(ip_packet);
    }
}