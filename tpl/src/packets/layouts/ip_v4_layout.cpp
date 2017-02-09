//
// Created by ukheg on 01.10.2016.
//

#include "tpl/packets/layouts/ip_v4_layout.hpp"

namespace tpl {
    namespace packets {
        namespace layouts {
            const std::shared_ptr<ip_v4_layout> ip_v4_layout::instance 
        		= std::make_shared<ip_v4_layout>();

            byte_array ip_v4_layout::header_bytes(const byte_array &bytes) const {
                return bytes.prefix(header_length(bytes));
            }

            byte_array ip_v4_layout::payload_bytes(const byte_array &bytes) const {
                auto h_length = header_length(bytes);
                return bytes.middle(h_length, total_length(bytes) - h_length);
            }

            uint16_t ip_v4_layout::total_length(byte_array const& bytes) const {
                return fields::total_length::get(bytes);
            }

            boost::asio::ip::address_v4 ip_v4_layout::source(byte_array const &bytes) const {
                auto addr_bytes= fields::source_addr::get_array(bytes);
                return boost::asio::ip::address_v4(addr_bytes);
            }

            boost::asio::ip::address ip_v4_layout::source_address(byte_array const &bytes) const {
                return this->source(bytes);
            }

            boost::asio::ip::address_v4 ip_v4_layout::destination(byte_array const &bytes) const {
                auto addr_bytes = fields::destination_addr::get_array(bytes);
                return boost::asio::ip::address_v4(addr_bytes);
            }

            boost::asio::ip::address ip_v4_layout::destination_address(byte_array const &bytes) const {
                return this->destination(bytes);
            }

	        object_id const& ip_v4_layout::get_packet_id() const
	        {
				return objects_id_repository::id_of<ip_v4_packet>();
	        }

	        ip_payload_protocols ip_v4_layout::next_header(byte_array const &bytes) const {
                return fields::protocol::get(bytes);
            }

            uint8_t ip_v4_layout::header_length(const byte_array &bytes) const {
                return (fields::version_length::get(bytes) & fields::length_mask) << 2;
            }


        }
    }
}