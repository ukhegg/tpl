//
// Created by ukheg on 05.10.2016.
//

#ifndef PLAIN_PACKETS_TRANSPORT_PACKET_LAYOUT_HPP
#define PLAIN_PACKETS_TRANSPORT_PACKET_LAYOUT_HPP

#include "tpl/packets/layouts/fields_layout.hpp"
#include "tpl/packets/transport/transport_type.hpp"

namespace tpl {
    namespace packets {
        namespace layouts {
            class transport_packet_layout : public fields_layout{
            public:
                virtual transport_type type(byte_array const& bytes) const = 0;

                virtual uint16_t src_port(byte_array const& bytes) const = 0;

                virtual uint16_t dst_port(byte_array const& bytes) const = 0;


            protected:

	            object_id const& get_packet_id() const override;
            };
        }
    }
}


#endif //PLAIN_PACKETS_TRANSPORT_PACKET_LAYOUT_HPP
