//
// Created by ukheg on 26.09.2016.
//

#ifndef PLAIN_PACKETS_IP_V4_PACKET_HPP
#define PLAIN_PACKETS_IP_V4_PACKET_HPP

#include "tpl/packets/data_link/ethernet_packet.hpp"
#include "tpl/packets/gre_packet.hpp"
#include "ip_packet.hpp"
#include "ip_v4_flags.hpp"


namespace tpl
{
	namespace packets
	{
		namespace layouts
		{
			class ip_v4_layout;
		}

		class TPL_API ip_v4_packet : public ip_packet
		{
			HAS_LAYOUT_OF_TYPE(layouts::ip_v4_layout)

		public:
			struct fields
			{
				using version_length = field_descriptor<uint8_t>;
				using diffirentiated_services = field_descriptor<uint8_t, version_length>;
				using total_length = field_descriptor<uint16_t, diffirentiated_services>;
				using id = field_descriptor<uint16_t, total_length>;
				using fragment_offset_and_flags = field_descriptor<uint16_t, id>;
				using ttl = field_descriptor<uint8_t, fragment_offset_and_flags>;
				using protocol = field_descriptor<ip_payload_protocols, ttl>;
				using checksum = field_descriptor<uint16_t, protocol>;
				using source_addr = field_descriptor<details::byte_sequence<4>, checksum>;
				using destination_addr = field_descriptor<details::byte_sequence<4>, source_addr>;

				static const int options_position = destination_addr::position + destination_addr::size;

				static const uint8_t version_mask = ip_packet::fields::version_mask;
				static const uint8_t length_mask = 0x0f;
				static const uint8_t flags_mask = 0xe0;
				static const uint16_t fragment_offset_mask = 0x1fff;

				static const uint16_t min_header_length = 20;
			};

			explicit ip_v4_packet(byte_array const& bytes);

			ip_v4_packet(byte_array const& bytes, layout_ptr const& layout);

			explicit ip_v4_packet(placeholders::parent_packet_holder<ip_packet> parent);

			explicit ip_v4_packet(placeholders::parent_packet_holder<ethernet_packet> parent);

			explicit ip_v4_packet(placeholders::parent_packet_holder<gre_packet> parent);

			uint8_t header_length() const;

			uint8_t differentiated_services() const;

			uint16_t total_length() const;

			uint16_t identification() const;

			ip_v4_flags flags() const;

			uint16_t fragment_offset() const;

			uint8_t ttl() const;

			uint16_t header_checksum() const;

			byte_array options_bytes() const;

			boost::asio::ip::address_v4 source() const;

			boost::asio::ip::address_v4 destination() const;

			static uint16_t calculate_header_checksum(const ip_v4_packet& packet);

			static uint16_t calculate_header_checksum(byte_array const& packet_bytes);

			static uint16_t calculate_checksum(byte_array const& packet);
		};
		

		PACKET_CAST_DECLARATION(ip_v4_packet);
	}
}
REGISTER_TPL_OBJECT(tpl::packets::ip_v4_packet, tpl::packets::ip_packet);

#endif //PLAIN_PACKETS_IP_V4_PACKET_HPP


