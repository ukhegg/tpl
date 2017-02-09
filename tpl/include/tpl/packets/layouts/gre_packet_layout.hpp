#pragma once

#include <tpl/utils/bit_converter.hpp>
#include "tpl/packets/gre_packet.hpp"
#include "tpl/packets/layouts/fields_layout.hpp"

namespace tpl {
    namespace packets {
        namespace layouts {
            class gre_packet_layout_base : public fields_layout {
            public:
                virtual boost::optional<uint16_t> checksum(byte_array const &bytes) const = 0;

                virtual boost::optional<uint16_t> offset(byte_array const &bytes) const = 0;

                virtual boost::optional<uint32_t> key(byte_array const &bytes) const = 0;

                virtual boost::optional<uint32_t> seq_number(byte_array const &bytes) const = 0;

                virtual byte_array routing_optional_bytes(byte_array const &bytes) const = 0;

                static const std::array<std::shared_ptr<gre_packet_layout_base>, 16> instances;

                static std::shared_ptr<gre_packet_layout_base> select_layout(gre_flags flags);


            protected:

	            object_id const& get_packet_id() const override;
            };

            //c-checksum present
            //r-routing present->need checksum
            //k-key present
            //s-sequence number os present
            constexpr size_t checksum_offset(bool c, bool r, bool k, bool s) {
                return gre_packet::fields::checksum::position;
            }

            constexpr size_t offset_offset(bool c, bool r, bool k, bool s) {
                return checksum_offset(c, r, c, s) + (c ? sizeof(uint16_t) : 0);
            }

            constexpr size_t key_offset(bool c, bool r, bool k, bool s) {
                return offset_offset(c, r, c, s) + ((c || r) ? sizeof(uint16_t) : 0);
            }

            constexpr size_t seq_number_offset(bool c, bool r, bool k, bool s) {
                return key_offset(c, r, c, s) + (k ? sizeof(uint16_t) : 0);
            }

            constexpr size_t routing_options_offset(bool c, bool r, bool k, bool s) {
                return seq_number_offset(c, r, c, s) + (s ? sizeof(uint16_t) : 0);
            }

            template<bool c, bool r, bool k, bool s>
            class gre_packet_layout : public gre_packet_layout_base {
            public:
                static const size_t optional_fields_begin = gre_packet::fields::checksum::position;

                boost::optional<uint16_t> checksum(byte_array const &bytes) const override {
                    if (c) return utils::ntoh<uint16_t>(bytes, checksum_offset(c, r, c, s));
                    return boost::none;
                }

                boost::optional<uint16_t> offset(byte_array const &bytes) const override {
                    if (c) return utils::ntoh<uint16_t>(bytes, offset_offset(c, r, c, s));
                    return boost::none;
                }

                boost::optional<uint32_t> key(byte_array const &bytes) const override {
                    if (k) return utils::ntoh<uint32_t>(bytes, key_offset(c, r, c, s));
                    return boost::none;
                }

                boost::optional<uint32_t> seq_number(byte_array const &bytes) const override {
                    if (s) return utils::ntoh<uint32_t>(bytes, key_offset(c, r, c, s));
                    return boost::none;
                }

                byte_array routing_optional_bytes(byte_array const &bytes) const override {
                    if (!r) return byte_array();
                    auto routing_offset = routing_options_offset(c, r, c, s);

                    size_t routing_size_ = 0;
                    auto routing_data = bytes.suffix(routing_offset);
                    gre_sre next;
                    do {
                        auto next_sre_length = gre_sre::length_static(routing_data);
                        const static auto sre_header_length = 4;
                        routing_size_ += next_sre_length + sre_header_length;
                        routing_data = routing_data.suffix(next_sre_length);
                    } while (next.sre_length() != 0);
                    return bytes.prefix(routing_size_);
                }

                byte_array header_bytes(byte_array const &bytes) const override {
                    return bytes.prefix(routing_options_offset(c, r, c, s) + routing_optional_bytes(bytes).size());
                }

                byte_array payload_bytes(byte_array const &bytes) const override {
                    return bytes.suffix(routing_options_offset(c, r, k, s) + routing_optional_bytes(bytes).size());
                }
            };



            class enhanced_gre_packet_layout : public fields_layout {
            public:
                using fields = enhanced_gre_packet::fields;

                static const std::shared_ptr<enhanced_gre_packet_layout> instance;

                byte_array header_bytes(byte_array const &bytes) const override;

                byte_array payload_bytes(byte_array const &bytes) const override;

                uint16_t payload_length(byte_array const &bytes) const;

                uint16_t call_id(byte_array const &bytes) const;

                uint32_t seq_number(byte_array const &bytes) const;

                uint32_t ack_number(byte_array const &bytes) const;


            protected:

	            object_id const& get_packet_id() const override;
            };


        }
    }
}

