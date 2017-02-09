//
// Created by ukheg on 01.10.2016.
//

#include "tpl/packets/layouts/ip_v6_layout.hpp"


namespace tpl {
    namespace packets {
        namespace layouts {
            const std::shared_ptr<ip_v6_layout> ip_v6_layout::instance = std::make_shared<ip_v6_layout>();

            byte_array ip_v6_layout::header_bytes(const byte_array &bytes) const {
                return bytes.prefix(fields::header_length);
            }

            byte_array ip_v6_layout::payload_bytes(const byte_array &bytes) const {
                return bytes.suffix(fields::header_length);
            }

            ip_version ip_v6_layout::version(byte_array const &bytes) const {
                return ip_packet_layout::version(bytes);
            }

            boost::asio::ip::address ip_v6_layout::source_address(byte_array const &bytes) const {
                return this->source(bytes);
            }

            boost::asio::ip::address ip_v6_layout::destination_address(byte_array const &bytes) const {
                return this->destination(bytes);
            }

            ip_payload_protocols ip_v6_layout::next_header(byte_array const &bytes) const {
                return ip_payload_protocols(bytes[ip_v6_packet::fields::next_header::position]);
            }

            boost::asio::ip::address_v6 ip_v6_layout::source(byte_array const &bytes) const {
                auto address_bytes = ip_v6_packet::fields::source_addr::get_array(bytes);
                return boost::asio::ip::address_v6(address_bytes);
            }

            boost::asio::ip::address_v6 ip_v6_layout::destination(byte_array const &bytes) const {
                auto address_bytes = ip_v6_packet::fields::destination_addr::get_array(bytes);
                return boost::asio::ip::address_v6(address_bytes);
            }

	        object_id const& ip_v6_layout::get_packet_id() const
	        {
				return objects_id_repository::id_of<ip_v6_packet>();
	        }
        }
    }
}