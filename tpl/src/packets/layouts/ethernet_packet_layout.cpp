//
// Created by ukheg on 29.09.2016.
//

#include "tpl/packets/layouts/ethernet_packet_layout.hpp"

namespace tpl{
    namespace packets{
        namespace layouts{
            const std::shared_ptr<ethernet_packet_layout> ethernet_packet_layout::instance = std::make_shared<ethernet_packet_layout>();

            byte_array ethernet_packet_layout::header_bytes(const byte_array &bytes) const {
                return bytes.prefix(fields::header_length);
            }

            byte_array ethernet_packet_layout::payload_bytes(const byte_array &bytes) const {
                return bytes.suffix(fields::header_length);
            }

            utils::mac_address ethernet_packet_layout::source_mac(const byte_array &bytes) const {
                return utils::mac_address(fields::source_mac::get(bytes));
            }

            utils::mac_address ethernet_packet_layout::destination_mac(byte_array const &bytes) const {
                return utils::mac_address(fields::destination_mac::get(bytes));
            }

            ethernet_payload_types ethernet_packet_layout::payload_type(byte_array const &bytes) const {
                return fields::payload_type::get(bytes);
            }

	        object_id const& ethernet_packet_layout::get_packet_id() const
	        {
				return objects_id_repository::id_of<ethernet_packet>();
	        }
        }
    }
}