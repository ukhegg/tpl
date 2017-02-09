//
// Created by ukheg on 01.10.2016.
//

#include "tpl/packets/ip_v6_packet.hpp"
#include "tpl/packets/layouts/ip_v6_layout.hpp"

namespace tpl {
    namespace packets {
        ip_v6_packet::ip_v6_packet(const byte_array &bytes) :
                ip_packet(bytes, layouts::ip_v6_layout::instance) {
        }

	    ip_v6_packet::ip_v6_packet(const byte_array& bytes, layout_ptr const& layout)
			: ip_packet(bytes, layout)
	    {
	    }

	    ip_v6_packet::ip_v6_packet(placeholders::parent_packet_holder<ip_packet> const& parent) 
			: ip_v6_packet(parent.packet.payload_bytes())
        {
	        if(parent.packet.next_header() != ip_payload_protocols::ipv6)
	        {
				throw std::invalid_argument("next header is not ip v6");
	        }
        }

	    ip_v6_packet::ip_v6_packet(placeholders::parent_packet_holder<ethernet_packet> const& parent) 
			: ip_v6_packet(parent.packet.payload_bytes())
        {
	        if(parent.packet.payload_type() != ethernet_payload_types::ip_v6)
	        {
				throw std::invalid_argument("next header is not ip v6");
	        }
        }

	    ip_v6_packet::ip_v6_packet(placeholders::parent_packet_holder<ah_packet> const& parent) 
			: ip_v6_packet(parent.packet.payload_bytes())
        {
			if (parent.packet.next_header() != ip_payload_protocols::ipv6)
			{
				throw std::invalid_argument("next header is not ip v6");
			}
        }

	    uint8_t ip_v6_packet::traffic_class() const {
            return reinterpret_cast<const version_traffic_class_and_flow_label_tuple *>(
                    &this->bytes_[fields::version_traffic_and_flow_label::position])->traffic_class;
        }

        uint32_t ip_v6_packet::flow_label() const {
            return reinterpret_cast<const uint32_t *>(
                           &this->bytes_[fields::version_traffic_and_flow_label::position])[0] &
                   0xfffff000;
        }

        uint16_t ip_v6_packet::payload_length() const {
            return fields::payload_length::get(this->bytes_);
        }

        uint8_t ip_v6_packet::hop_limit() const {
            return fields::hop_limit::get(this->bytes_);
        }

        boost::asio::ip::address_v6 ip_v6_packet::source() const {
            return this->layout().source(this->bytes_);
        }

        boost::asio::ip::address_v6 ip_v6_packet::destination() const {
            return this->layout().destination(this->bytes_);
        }

        ip_v6_extension_headers_sequence const& ip_v6_packet::extension_headers() const {
            if (this->extension_headers_ == boost::none) {
                this->extension_headers_ = this->read_extension_headers();
            }
            return *this->extension_headers_;
        }

        ip_v6_extension_headers_sequence ip_v6_packet::read_extension_headers() const {
            size_t extensions_length = 0;
            auto calculate_ext_headers_sum_length = [&extensions_length](ip_v6_extension_header const &ext_header) {
                extensions_length += ext_header.total_length();
            };

            auto payload_bytes = this->payload_bytes();
            ip_v6_extension_headers_sequence::enumerate_extension_headers(payload_bytes,
                                                                          this->next_header(),
                                                                          calculate_ext_headers_sum_length);
            return ip_v6_extension_headers_sequence(payload_bytes.prefix(extensions_length), this->next_header());
        }

		PACKET_CAST_DEFINITION(ip_v6_packet);
    }
}