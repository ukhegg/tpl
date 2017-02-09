//
// Created by ukheg on 26.09.2016.
//

#include <tpl/packets/layouts/ethernet_packet_layout.hpp>
#include "tpl/packets/data_link/ethernet_packet.hpp"


namespace tpl {
    namespace packets {

        ethernet_packet::ethernet_packet(byte_array const &bytes)
                : packet(bytes, layouts::ethernet_packet_layout::instance) {}

        ethernet_packet::ethernet_packet(byte_array const &bytes, layout_ptr const &layout)
                : packet(bytes, layout) {
        }

        utils::mac_address ethernet_packet::source_mac() const {
            return this->layout().source_mac(this->bytes_);
        }

        utils::mac_address ethernet_packet::destination_mac() const {
            return this->layout().destination_mac(this->bytes_);
        }

        ethernet_payload_types ethernet_packet::payload_type() const {
            return this->layout().payload_type(this->bytes_);
        }

        template<>
        ethernet_packet packet::as() const {
            auto layout = std::dynamic_pointer_cast<ethernet_packet::layout_type>(this->layout_);
            if (layout == nullptr) throw std::bad_cast();
            return ethernet_packet(this->bytes_, layout);
        }
    }
}