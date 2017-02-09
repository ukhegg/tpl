#pragma once

#include "transport_packet.hpp"
#include "tpl/packets/ip_packet.hpp"
#include "tcp_options_sequence.hpp"

namespace tpl {
    namespace packets {
        namespace layouts {
            class tcp_packet_layout;
        }

        struct TPL_API tcp_flags {
	        explicit tcp_flags(uint16_t length_and_flags);

            bool fin : 1;
            bool syn : 1;
            bool reset : 1;
            bool push : 1;
            bool ack : 1;
            bool urg : 1;
            bool ecn_echo : 1;
            bool cwr : 1;
            bool nonce : 1;
            uint8_t reserved : 3;
            uint8_t length : 4;


	        friend TPL_API bool operator==(tcp_flags const& lhs, tcp_flags const& rhs);

	        friend TPL_API bool operator!=(tcp_flags const& lhs, tcp_flags const& rhs);
        };

        class TPL_API tcp_packet : public transport_packet {
            HAS_LAYOUT_OF_TYPE(layouts::tcp_packet_layout);
        public:
            struct fields {
                using source_port = field_descriptor<uint16_t>;
                using destination_port = field_descriptor<uint16_t, source_port>;
                using seq_number = field_descriptor<uint32_t, destination_port>;
                using ack_number = field_descriptor<uint32_t, seq_number>;
                using header_length_and_flags = field_descriptor<uint16_t, ack_number>;
                using window_size = field_descriptor<uint16_t, header_length_and_flags>;
                using checksum = field_descriptor<uint16_t, window_size>;
                using urgent_pointer = field_descriptor<uint16_t, checksum>;

                static const auto header_base_length = urgent_pointer::position + urgent_pointer::size;
                static const uint16_t length_mask = 0xf000;
                static const uint16_t flags_mask = 0x0fff;
                static const int options_position = urgent_pointer::position + urgent_pointer::size;
            };

            tcp_packet();

            explicit tcp_packet(byte_array const &bytes);

            tcp_packet(byte_array const &bytes, layout_ptr const& layout);

            explicit tcp_packet(placeholders::parent_packet_holder<ip_packet> const &parent_packet);

            static boost::optional<tcp_packet> try_create(placeholders::parent_packet_holder<ip_packet> const &parent_packet);

            virtual ~tcp_packet();

            uint32_t seq_number() const;

            uint32_t ack_number() const;

            uint8_t header_length() const;

            tcp_flags flags() const;

            uint16_t windows_size() const;

            uint16_t checksum() const;

            uint16_t urgent_pointer() const;

            tcp_options_sequence options() const;

            static uint16_t calculate_checksum(tcp_packet const &packet,
                                               boost::asio::ip::address_v4 const &src,
                                               boost::asio::ip::address_v4 const &dst);

        };

		PACKET_CAST_DECLARATION(tcp_packet);
    }
}
REGISTER_TPL_OBJECT(tpl::packets::tcp_packet, tpl::packets::transport_packet);
