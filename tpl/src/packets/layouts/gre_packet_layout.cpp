#include "tpl/packets/layouts/gre_packet_layout.hpp"
#include "tpl/utils/bit_converter.hpp"

namespace tpl
{
	namespace packets
	{
		namespace layouts
		{
			//////////////////////////////////////////////////////////////////////////
			// gre version 0
			//////////////////////////////////////////////////////////////////////////

			std::shared_ptr<gre_packet_layout_base> gre_packet_layout_base::select_layout(gre_flags flags)
			{
				uint16_t raw_value = *reinterpret_cast<uint16_t *>(&flags);
				auto layout_index = raw_value >> 12;
				return gre_packet_layout_base::instances[layout_index];
			}

			object_id const& gre_packet_layout_base::get_packet_id() const
			{
				return objects_id_repository::id_of<gre_packet>();
			}

			//select instance based on flags
			const std::array<std::shared_ptr<gre_packet_layout_base>, 16> gre_packet_layout_base::instances = []()
			{
				std::array<std::shared_ptr<gre_packet_layout_base>, 16> res;
				res[0b00000000 >> 4] = std::make_shared<gre_packet_layout<false, false, false, false>>();
				res[0b00010000 >> 4] = std::make_shared<gre_packet_layout<false, false, false, true>>();
				res[0b00100000 >> 4] = std::make_shared<gre_packet_layout<false, false, true, false>>();
				res[0b00110000 >> 4] = std::make_shared<gre_packet_layout<false, false, true, true>>();
				res[0b01000000 >> 4] = std::make_shared<gre_packet_layout<false, true, false, false>>();
				res[0b01010000 >> 4] = std::make_shared<gre_packet_layout<false, true, false, true>>();
				res[0b01100000 >> 4] = std::make_shared<gre_packet_layout<false, true, true, false>>();
				res[0b01110000 >> 4] = std::make_shared<gre_packet_layout<false, true, true, true>>();
				res[0b10000000 >> 4] = std::make_shared<gre_packet_layout<true, false, false, false>>();
				res[0b10010000 >> 4] = std::make_shared<gre_packet_layout<true, false, false, true>>();
				res[0b10100000 >> 4] = std::make_shared<gre_packet_layout<true, false, true, false>>();
				res[0b10110000 >> 4] = std::make_shared<gre_packet_layout<true, false, true, true>>();
				res[0b11000000 >> 4] = std::make_shared<gre_packet_layout<true, true, false, false>>();
				res[0b11010000 >> 4] = std::make_shared<gre_packet_layout<true, true, false, true>>();
				res[0b11100000 >> 4] = std::make_shared<gre_packet_layout<true, true, true, false>>();
				res[0b11110000 >> 4] = std::make_shared<gre_packet_layout<true, true, true, true>>();
				return res;
			}();

			//////////////////////////////////////////////////////////////////////////
			// enchansed gre
			//////////////////////////////////////////////////////////////////////////
			const std::shared_ptr<enhanced_gre_packet_layout> enhanced_gre_packet_layout::instance
			= std::make_shared<enhanced_gre_packet_layout>();

			byte_array enhanced_gre_packet_layout::header_bytes(const byte_array& bytes) const
			{
				auto flags = gre_flags(fields::flags::get(bytes));
				size_t header_size = enhanced_gre_packet_layout::fields::header_min_length;
				if (flags.ack_number_present()) header_size += fields::ack_number::size;
				if (flags.seq_number_present()) header_size += fields::seq_number::size;
				return bytes.prefix(header_size);
			}

			byte_array enhanced_gre_packet_layout::payload_bytes(const byte_array& bytes) const
			{
				auto flags = gre_flags(fields::flags::get(bytes));
				size_t header_size = enhanced_gre_packet_layout::fields::header_min_length;
				if (flags.ack_number_present()) header_size += fields::ack_number::size;
				if (flags.seq_number_present()) header_size += fields::seq_number::size;
				return bytes.suffix(header_size);
			}

			uint16_t enhanced_gre_packet_layout::payload_length(const byte_array& bytes) const
			{
				return fields::payload_length::get(bytes);
			}

			uint16_t enhanced_gre_packet_layout::call_id(const byte_array& bytes) const
			{
				return fields::call_id::get(bytes);
			}

			uint32_t enhanced_gre_packet_layout::ack_number(const byte_array& bytes) const
			{
				auto flags = gre_flags(fields::flags::get(bytes));
				if (!flags.ack_number_present())
				{
					throw std::runtime_error("ack number bit not set");
				}

				size_t ack_offset = flags.seq_number_present() ?
					                    fields::seq_number::position + fields::seq_number::size
					                    : fields::seq_number::position;

				return utils::ntoh<uint32_t>(bytes, ack_offset);
			}

			object_id const& enhanced_gre_packet_layout::get_packet_id() const
			{
				return objects_id_repository::id_of<enhanced_gre_packet>();
			}

			uint32_t enhanced_gre_packet_layout::seq_number(const byte_array& bytes) const
			{
				auto flags = gre_flags(fields::flags::get(bytes));
				if (!flags.seq_number_present())
				{
					throw std::runtime_error("seq number bit not set");
				}

				return fields::seq_number::get(bytes);
			}
		}
	}
}
