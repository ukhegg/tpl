//
// Created by ukheg on 05.10.2016.
//

#include "tpl/packets/transport/udp_packet.hpp"
#include "tpl/packets/layouts/transport_packet_layout.hpp"
#include <tpl/utils/bit_converter.hpp>

namespace tpl
{
	namespace packets
	{
		namespace layouts
		{
			class udp_packet_layout : public transport_packet_layout
			{
			public:
				using fields = udp_packet::fields;

				static const std::shared_ptr<udp_packet_layout> instance;

				virtual transport_type type(byte_array const& bytes) const override
				{
					return transport_type::udp;
				}

				virtual byte_array header_bytes(const byte_array& bytes) const override
				{
					return bytes.prefix(fields::header_length);
				}

				virtual byte_array payload_bytes(const byte_array& bytes) const override
				{
					return bytes.middle(fields::header_length,
					                    length(bytes) - udp_packet::fields::header_length);
				}

				virtual uint16_t src_port(byte_array const& bytes) const override
				{
					return fields::source_port::get(bytes);
				}

				virtual uint16_t dst_port(byte_array const& bytes) const override
				{
					return fields::destination_port::get(bytes);
				}

				uint16_t length(byte_array const& bytes) const
				{
					return fields::length::get(bytes);
				}


			protected:
				object_id const& get_packet_id() const override{
					return objects_id_repository::id_of<udp_packet>();
				}
			};

			const std::shared_ptr<udp_packet_layout> udp_packet_layout::instance =
			std::make_shared<udp_packet_layout>();
		}

		udp_packet::udp_packet()
		{
		}

		udp_packet::udp_packet(const byte_array& bytes)
			: transport_packet(bytes, layouts::udp_packet_layout::instance)
		{
			if (this->bytes_.size() < fields::header_length)
			{
				throw std::invalid_argument("not enough bytes for an udp packet");
			}
		}

		udp_packet::udp_packet(byte_array const& bytes, layout_ptr const& layout)
			: transport_packet(bytes, layout)
		{
			if (this->bytes_.size() < fields::header_length)
			{
				throw std::invalid_argument("not enough bytes for an udp packet");
			}
		}

		udp_packet::udp_packet(placeholders::parent_packet_holder<ip_packet> parent)
			: udp_packet(parent.packet.payload_bytes())
		{
			if (parent.packet.next_header() != ip_payload_protocols::udp)
			{
				throw std::invalid_argument("next header is not udp");
			}
		}

		uint16_t udp_packet::length() const
		{
			return this->layout().length(this->bytes_);
		}

		uint16_t udp_packet::checksum() const
		{
			return fields::checksum::get(this->bytes_);
		}

		uint16_t udp_packet::calculate_checksum(const udp_packet& packet, boost::asio::ip::address_v4 const& src, boost::asio::ip::address_v4 const& dst)
		{
			auto size = packet.length();

			uint8_t pseudo_header[12];
			memcpy(pseudo_header, src.to_bytes().data(), 4);
			memcpy(pseudo_header + 4, dst.to_bytes().data(), 4);
			pseudo_header[8] = 0;
			pseudo_header[9] = uint8_t(ip_payload_protocols::udp);
			utils::hton_unsafe<uint16_t>(pseudo_header, 10, size);

			uint32_t result = 0;
			for (size_t i = 0; i < 12; i += 2)
			{
				result += utils::ntoh_unsafe<uint16_t>(pseudo_header, i);
			}

			--size;
			auto packet_bytes = packet.bytes();
			for (size_t i = 0; i < size; i += 2)
			{
				result += utils::ntoh<uint16_t>(packet_bytes, i);
			}

			if (size % 2 == 0)
			{
				auto end = uint16_t(packet_bytes[size]) << 8;
				result += end;
			}

			result = (result >> 16) + (result & 0xffff);
			return ~uint16_t(result);
		}

		boost::optional<udp_packet> udp_packet::try_create(placeholders::parent_packet_holder<ip_packet> parent)
		{
			return try_create(parent.packet.payload_bytes());
		}

		boost::optional<udp_packet> udp_packet::try_create(byte_array const& bytes)
		{
			if (bytes.size() < fields::header_length) return boost::none;
			return udp_packet(bytes);
		}

		PACKET_CAST_DEFINITION(udp_packet);
	}
}
