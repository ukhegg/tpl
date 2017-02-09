#include "tpl/packets/transport/tcp_packet.hpp"
#include <boost/pool/pool_alloc.hpp>
#include <tpl/utils/bit_converter.hpp>
#include "tpl/packets/layouts/transport_packet_layout.hpp"

namespace tpl {
    namespace packets {
        namespace layouts {
            class tcp_packet_layout : public transport_packet_layout {
            public:
                using fields = tcp_packet::fields;

                static const std::shared_ptr<tcp_packet_layout> instance;

                virtual transport_type type(byte_array const &bytes) const override{
                    return transport_type::tcp;
                }

                virtual byte_array header_bytes(const byte_array &bytes) const override{
                    return bytes.prefix(this->header_length(bytes));
                }

                virtual byte_array payload_bytes(const byte_array &bytes) const override{
                    return bytes.suffix(this->header_length(bytes));
                }

                virtual uint16_t src_port(byte_array const &bytes) const override{
                    return fields::source_port::get(bytes);
                }

                virtual uint16_t dst_port(byte_array const &bytes) const override{
                    return fields::destination_port::get(bytes);
                }

                uint8_t header_length(byte_array const &bytes) const {
                    uint16_t length_and_flags = fields::header_length_and_flags::get(bytes);
                    return (length_and_flags >> 12) * 4;
                }


            protected:

	            object_id const& get_packet_id() const override{
					return objects_id_repository::id_of<tcp_packet>();
				}
            };

            const std::shared_ptr<tcp_packet_layout> tcp_packet_layout::instance =
                    std::make_shared<tcp_packet_layout>();
        }
        struct flags_and_length_field {
            explicit flags_and_length_field(uint16_t r_value) : raw_value(r_value) {}

            tcp_flags flags() const {
                return tcp_flags(raw_value);
            }

            uint16_t length() const {
                return (raw_value >> 12) * 4;
            }

            uint16_t raw_value;
        };

	    tcp_flags::tcp_flags(uint16_t length_and_flags)
	    {
		    static_assert(sizeof(tcp_flags) == sizeof(uint16_t), "invalid tcp_flags alignment");
		    reinterpret_cast<uint16_t *>(this)[0] = length_and_flags;
	    }

	    tcp_packet::tcp_packet() : transport_packet(){}

        tcp_packet::tcp_packet(byte_array const &bytes)
                : transport_packet(bytes, layouts::tcp_packet_layout::instance) {
            if(bytes.size() < fields::header_base_length){
                throw std::invalid_argument("invalid tcp header-too short");
            }
        }

	    tcp_packet::tcp_packet(byte_array const& bytes, layout_ptr const& layout)
			: transport_packet(bytes, layout)
	    {
	    }

	    tcp_packet::tcp_packet(placeholders::parent_packet_holder<ip_packet> const &parent_packet)
                : tcp_packet(parent_packet.packet.payload_bytes()) {
        }

        boost::optional<tcp_packet> tcp_packet::try_create(placeholders::parent_packet_holder<ip_packet> const &parent_packet) {
            auto bytes = parent_packet.packet.payload_bytes();
            if(bytes.size() < fields::header_base_length){
				return boost::none;
            }
			auto length = layouts::tcp_packet_layout::instance->header_length(bytes);
			if (bytes.size() < length) return  boost::none;
            return tcp_packet(bytes);
        }

        tcp_packet::~tcp_packet() {}

        uint32_t tcp_packet::seq_number() const {
            return fields::seq_number::get(this->bytes_);
        }

        uint32_t tcp_packet::ack_number() const {
            return fields::ack_number::get(this->bytes_);
        }

        uint8_t tcp_packet::header_length() const {
            return this->layout().header_length(this->bytes_);
        }

        tcp_flags tcp_packet::flags() const {
            uint16_t length_and_flags = fields::header_length_and_flags::get(this->bytes_);
            return tcp_flags(length_and_flags);
        }

        uint16_t tcp_packet::windows_size() const {
            return fields::window_size::get(this->bytes_);
        }

        uint16_t tcp_packet::checksum() const {
            return fields::checksum::get(this->bytes_);
        }

        uint16_t tcp_packet::urgent_pointer() const {
            return fields::urgent_pointer::get(this->bytes_);
        }


        tcp_options_sequence tcp_packet::options() const {
            return tcp_options_sequence(this->header_bytes().suffix(tcp_packet::fields::options_position));
        }

        uint16_t tcp_packet::calculate_checksum(const tcp_packet &packet,
                                                boost::asio::ip::address_v4 const &src,
                                                boost::asio::ip::address_v4 const &dst) {
            auto size = packet.bytes().size();

            uint8_t pseudo_header[12];
            memcpy(pseudo_header, src.to_bytes().data(), 4);
            memcpy(pseudo_header + 4, dst.to_bytes().data(), 4);
            pseudo_header[8] = 0;
            pseudo_header[9] = uint8_t(ip_payload_protocols::tcp);
            if (size > std::numeric_limits<uint16_t>::max()) {
                throw std::invalid_argument("size > max packet length");
            }
            utils::hton_unsafe<uint16_t>(pseudo_header, 10, static_cast<uint16_t>(size));

            uint32_t result = 0;
            for (size_t i = 0; i < 12; i += 2) {
                result += utils::ntoh_unsafe<uint16_t>(pseudo_header, i);
            }

            --size;
            auto packet_bytes = packet.bytes();
            for (size_t i = 0; i < size; i += 2) {
                result += utils::ntoh<uint16_t>(packet_bytes, i);
            }

            if (size % 2 == 0) {
                auto end = uint16_t(packet_bytes[size]) << 8;
                result += end;
            }

            result = (result >> 16) + (result & 0xffff);
            return ~uint16_t(result);
        }

	    bool operator==(tcp_flags const& lhs, tcp_flags const& rhs)
	    {
		    return reinterpret_cast<uint16_t const *>(&lhs)[0] == reinterpret_cast<uint16_t const *>(&rhs)[0];
	    }

	    bool operator!=(tcp_flags const& lhs, tcp_flags const& rhs)
	    {
		    return !(lhs == rhs);
	    }

		PACKET_CAST_DEFINITION(tcp_packet);
    }
}

