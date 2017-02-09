//
// Created by ukheg on 26.09.2016.
//

#include "tpl/packets/ip_v4_packet.hpp"
#include "tpl/packets/layouts/ip_v4_layout.hpp"

namespace tpl
{
	namespace packets
	{
		ip_v4_packet::ip_v4_packet(byte_array const& bytes) : ip_packet(bytes, layouts::ip_v4_layout::instance) {}

		ip_v4_packet::ip_v4_packet(byte_array const& bytes, layout_ptr const& layout)
			: ip_packet(bytes, layout)
		{
		}

		ip_v4_packet::ip_v4_packet(placeholders::parent_packet_holder<ip_packet> parent)
			: ip_v4_packet(parent.packet.payload_bytes())
		{
			if(parent.packet.next_header() != ip_payload_protocols::ipip)
			{
				throw std::invalid_argument("next header is not ip v4");
			}
		}

		ip_v4_packet::ip_v4_packet(placeholders::parent_packet_holder<ethernet_packet> parent)
			: ip_packet(parent.packet.payload_bytes(), layouts::ip_v4_layout::instance)
		{
			if(parent.packet.payload_type() != ethernet_payload_types::ip)
			{
				throw std::invalid_argument("next header is not ip v4");
			}
		}

		ip_v4_packet::ip_v4_packet(placeholders::parent_packet_holder<gre_packet> parent)
			: ip_packet(parent.packet.payload_bytes(), layouts::ip_v4_layout::instance)
		{
			if(parent.packet.payload_protocol() != gre_payload_protocol::ip)
			{
				throw std::invalid_argument("payload protocol is not ip v4");
			}
		}

		uint8_t ip_v4_packet::header_length() const
		{
			return this->layout().header_length(this->bytes_);
		}

		uint8_t ip_v4_packet::differentiated_services() const
		{
			return fields::diffirentiated_services::get(this->bytes_);
		}

		uint16_t ip_v4_packet::total_length() const
		{
			return this->layout().total_length(this->bytes_);
		}

		uint16_t ip_v4_packet::identification() const
		{
			return fields::id::get(this->bytes_);
		}

		ip_v4_flags ip_v4_packet::flags() const
		{
			auto foaf = fields::fragment_offset_and_flags::get(this->bytes_) >> 8;
			return packets::ip_v4_flags(foaf & fields::flags_mask);
		}

		uint16_t ip_v4_packet::fragment_offset() const
		{
			return fields::fragment_offset_and_flags::get(this->bytes_) & fields::fragment_offset_mask;
		}

		uint8_t ip_v4_packet::ttl() const
		{
			return fields::ttl::get(this->bytes_);
		}

		uint16_t ip_v4_packet::header_checksum() const
		{
			return fields::checksum::get(this->bytes_);
		}

		byte_array ip_v4_packet::options_bytes() const
		{
			return this->header_bytes().suffix(fields::options_position);
		}

		boost::asio::ip::address_v4 ip_v4_packet::source() const
		{
			return this->layout().source(this->bytes_);
		}

		boost::asio::ip::address_v4 ip_v4_packet::destination() const
		{
			return this->layout().destination(this->bytes_);
		}

		uint16_t ip_v4_packet::calculate_header_checksum(const ip_v4_packet& packet)
		{
			return calculate_checksum(packet.header_bytes().prefix(1));
		}

		uint16_t ip_v4_packet::calculate_header_checksum(byte_array const& packet_bytes)
		{
			auto header_length = packets::layouts::ip_v4_layout::instance->header_length(packet_bytes);
			return calculate_checksum(packet_bytes.prefix(header_length));
		}

		uint16_t ip_v4_packet::calculate_checksum(byte_array const& packet)
		{
			auto header_bytes = reinterpret_cast<const uint16_t *>(packet.data());
			if(packet.size() > std::numeric_limits<uint8_t>::max())
			{
				throw std::invalid_argument("packet too long");
			}

			auto header_length = static_cast<uint8_t>(packet.size());
			uint8_t position = 0;

			uint32_t sum = 0;

			while (position < header_length) {
				uint16_t val = 0;

				if (position != fields::checksum::position) {
					val = header_bytes[position / 2];
				}
				sum += val;
				position += 2;
			}

			while ((sum >> 16) != 0) {
				sum = (sum & 0xffff) + (sum >> 16);
			}

			if (sum == 0xffff) return sum;
			uint16_t result = ~sum;
			return (result >> 8) | (result << 8);
		}

		PACKET_CAST_DEFINITION(ip_v4_packet);
	}
}

