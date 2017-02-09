//
// Created by ukheg on 29.09.2016.
//

#ifndef PLAIN_PACKETS_ETHERNET_PACKET_LAYOUT_HPP
#define PLAIN_PACKETS_ETHERNET_PACKET_LAYOUT_HPP
#include "tpl/packets/data_link/ethernet_packet.hpp"

namespace tpl{
    namespace packets{
        namespace layouts{
            class ethernet_packet_layout : public fields_layout{
            public:
                using fields = ethernet_packet::fields;

                static const std::shared_ptr<ethernet_packet_layout> instance;

                virtual byte_array header_bytes(const byte_array &bytes) const override;

                virtual byte_array payload_bytes(const byte_array &bytes) const override;

                utils::mac_address source_mac(const byte_array& bytes) const;

                utils::mac_address destination_mac(byte_array const& bytes) const;

                ethernet_payload_types payload_type(byte_array const& bytes) const;


            protected:

	            object_id const& get_packet_id() const override;
            };
        }
    }
}


#endif //PLAIN_PACKETS_ETHERNET_PACKET_LAYOUT_HPP
