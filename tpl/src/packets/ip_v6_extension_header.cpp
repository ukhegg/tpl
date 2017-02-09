//
// Created by ukheg on 02.10.2016.
//

#include "tpl/packets/ip_v6_extension_header.hpp"


namespace tpl {
    namespace packets {
        namespace layouts {
            class ip_v6_extension_header_layout : public fields_layout {
            public:
                using fields = ip_v6_extension_header::fields;

                virtual byte_array header_bytes(const byte_array &bytes) const override {
                    return bytes.prefix(fields::header_length);
                }

                virtual byte_array payload_bytes(const byte_array &bytes) const override {
                    return bytes.suffix(fields::header_length);
                }

                ip_payload_protocols next_header(byte_array const &bytes) {
                    return fields::next_header::get(bytes);
                }

                virtual size_t total_length(byte_array const &bytes) const = 0;


            protected:
	            object_id const& get_packet_id() const override{
					return objects_id_repository::id_of<ip_v6_extension_header>();
				}
            };

            class uniform_extension_header_layout : public ip_v6_extension_header_layout {
            public:
                struct fields : ip_v6_extension_header_layout::fields {
                    using data_length = field_descriptor<uint8_t, next_header>;

                    static const size_t header_length = data_length::position + data_length::size;
                };

                static const std::shared_ptr<uniform_extension_header_layout> instance;

                virtual byte_array header_bytes(const byte_array &bytes) const override {
                    return bytes.prefix(fields::header_length);
                }

                virtual byte_array payload_bytes(const byte_array &bytes) const override {
                    return bytes.suffix(fields::header_length);
                }

                uint8_t data_length(byte_array const &bytes) const {
                    return 8 * fields::data_length::get(bytes);
                }

                virtual size_t total_length(byte_array const &bytes) const override {
                    return 8 + data_length(bytes);
                }
            };

            const std::shared_ptr<uniform_extension_header_layout> uniform_extension_header_layout::instance
                    = std::make_shared<uniform_extension_header_layout>();

            class fragment_extension_header_layout : public ip_v6_extension_header_layout {
            public:
                struct fields : ip_v6_extension_header_layout::fields {
                    using reserved = field_descriptor<uint8_t, next_header>;
                    using offset_and_flags = field_descriptor<uint16_t, reserved>;
                    using identification = field_descriptor<uint32_t, offset_and_flags>;

                    static const size_t total_length = fields::identification::position + fields::identification::size;
                };

                static const std::shared_ptr<fragment_extension_header_layout> instance;

                uint8_t reserved(byte_array const &bytes) const {
                    return fields::reserved::get(bytes);
                }

                uint16_t offset_and_flags(byte_array const &bytes) const {
                    return fields::offset_and_flags::get(bytes);
                }

                uint16_t offset(byte_array const &bytes) const {
                    uint16_t offset_mask = 0xfff8;
                    auto t = this->offset_and_flags(bytes) & offset_mask;
                }

                bool more_fragments(byte_array const &bytes) const {
                    uint16_t more_fragments_bit_mask = 0x0001;
                    return (this->offset_and_flags(bytes) & more_fragments_bit_mask) == more_fragments_bit_mask;
                }

                uint32_t identification(byte_array const &bytes) const {
                    return fields::identification::get(bytes);
                }

                virtual size_t total_length(byte_array const &bytes) const override {
                    return fields::total_length;
                }
            };

            const std::shared_ptr<fragment_extension_header_layout> fragment_extension_header_layout::instance
                    = std::make_shared<fragment_extension_header_layout>();

            std::shared_ptr<ip_v6_extension_header_layout> layout_for(ip_payload_protocols protocol) {
                switch (protocol) {
                    case ip_payload_protocols::fragment:
                        return fragment_extension_header_layout::instance;
                    case ip_payload_protocols::hopopts:
                    case ip_payload_protocols::dstopts:
                    case ip_payload_protocols::routing:
                        return uniform_extension_header_layout::instance;
                    default:
                        throw std::invalid_argument("invalid ipv6 extension header type");
                }
            }
        }

        ip_v6_extension_header::ip_v6_extension_header() : type_(ip_payload_protocols::none) {
        }

        ip_v6_extension_header::ip_v6_extension_header(byte_array const &bytes, ip_payload_protocols type) :
                packet(bytes, layouts::layout_for(type)),
                type_(type) {
        }

        size_t ip_v6_extension_header::total_length() const {
            return this->layout().total_length(this->bytes_);
        }

        ip_payload_protocols ip_v6_extension_header::next_header() const {
            return this->layout().next_header(this->bytes_);
        }

        bool ip_v6_extension_header::is_extension_header_type(ip_payload_protocols protocol) {
            switch (protocol) {
                case ip_payload_protocols::hopopts:
                case ip_payload_protocols::dstopts:
                case ip_payload_protocols::routing:
                case ip_payload_protocols::fragment:
                    return true;
                default:
                    return false;
            }
        }

        size_t ip_v6_extension_header::next_header_size(byte_array const &bytes, ip_payload_protocols header_type) {
            if(bytes.size() < ip_v6_extension_header::fields::header_length) return 0;
            if(!ip_v6_extension_header::is_extension_header_type(header_type)) return 0;

            return layouts::layout_for(header_type)->total_length(bytes);
        }
    }
}