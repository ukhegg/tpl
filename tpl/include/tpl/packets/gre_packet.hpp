#pragma once

#include "tpl/packets/packet.hpp"
#include "tpl/packets/ip_packet.hpp"

namespace tpl {
    namespace packets {
        namespace layouts {
            class fields_layout;

            class gre_packet_layout_base;

            class enhanced_gre_packet_layout;
        }

        enum class gre_version : uint8_t {
            gre = 0,
            enhanced_gre = 1
        };

        enum class gre_payload_protocol : uint16_t {
            reserved = 0x0000,
            sna = 0x0004,
            osi_network_layer = 0x00fe,
            pup = 0x0200,
            xns = 0x0600,
            ip = 0x0800,
            chaos = 0x0804,
            rfc826_arp = 0x0806,
            frame_relay_arp = 0x0808,
            vines = 0x0bad,
            vines_echo = 0x0bae,
            vines_loopback = 0x0baaf,
            dec_net = 0x6003,
            transparent_ethernet_bridging = 0x6558,
            raw_frame_relay = 0x6559,
            appolo_domain = 0x8019,
            ethertalk = 0x809b,
            novell_ipx = 0x8137,
            rfc1144_tcp_ip_compression = 0x876b,
            ip_autonomous_system = 0x876c,
            secure_data = 0x876d,
            ppp = 0x880b
        };

        struct TPL_API gre_flags {
            gre_flags();

            explicit gre_flags(uint16_t value);

            gre_version version() const { return static_cast<gre_version>(this->version_field); }

            uint8_t other_flags() const { return this->other_flags_field; }

            bool ack_number_present() const { return this->ack_number_present_field; }

            uint8_t recursion_control() const { return this->recursion_control_field; }

            bool strict_source_route() const { return this->strict_source_route_field; }

            bool seq_number_present() const { return this->seq_number_present_field; }

            bool key_present() const { return this->key_present_field; }

            bool routing_present() const { return this->routing_present_field; }

            bool checksum_present() const { return this->checksum_present_field; }

        private:
            union {
                struct {
                    uint8_t version_field : 3;
                    uint8_t other_flags_field : 4;
                    bool ack_number_present_field : 1;
                    uint8_t recursion_control_field : 3;
                    bool strict_source_route_field : 1;
                    bool seq_number_present_field : 1;
                    bool key_present_field : 1;
                    bool routing_present_field : 1;
                    bool checksum_present_field : 1;
                };
                uint16_t raw_value;
            };
        };

        class TPL_API gre_packet_base : public packet {
        HAS_LAYOUT_OF_TYPE(layouts::fields_layout)

        public:
            struct fields {
                using flags = field_descriptor<uint16_t>;
                using protocol = field_descriptor<gre_payload_protocol, flags>;
            };

            gre_packet_base();

            explicit gre_packet_base(const byte_array &bytes);

            gre_packet_base(const byte_array &bytes, layout_ptr const &layout);

            explicit gre_packet_base(const placeholders::parent_packet_holder<packet> &parent_packet);

            virtual ~gre_packet_base();

            gre_flags flags() const;

            gre_payload_protocol payload_protocol() const;
        };

        PACKET_CAST_DECLARATION(gre_packet_base);

        //////////////////////////////////////////////////////////////////////////
        //
        //////////////////////////////////////////////////////////////////////////
        class TPL_API gre_packet : public gre_packet_base {
        HAS_LAYOUT_OF_TYPE(layouts::gre_packet_layout_base)

        public:
            struct fields : gre_packet_base::fields {
                using checksum = field_descriptor<uint16_t, protocol>;
                using offset = field_descriptor<uint16_t, checksum>;

                struct key {
                    static const int size = 4;
                };

                struct seq_number {
                    static const int size = 4;
                };
            };

            gre_packet();

            explicit gre_packet(const byte_array &bytes);

            gre_packet(const byte_array &bytes, layout_ptr const &layout);

            explicit gre_packet(const placeholders::parent_packet_holder<packet> &parent_packet);

            virtual ~gre_packet();

            boost::optional<uint16_t> checksum() const;

            boost::optional<uint16_t> offset() const;

            boost::optional<uint32_t> key() const;

            boost::optional<uint32_t> seq_number() const;
        };


        PACKET_CAST_DECLARATION(gre_packet);

        class TPL_API gre_sre {
        public:
            struct fields {
                using address_family = field_descriptor<uint16_t>;
                using sre_offset = field_descriptor<uint8_t, address_family>;
                using sre_length = field_descriptor<uint8_t, sre_offset>;

                struct routing_info {
                    static const size_t position = sre_length::position + sre_length::size;
                };
            };

            gre_sre();

            explicit gre_sre(const byte_array &bytes);

            uint16_t address_family() const;

            uint8_t sre_offset() const;

            uint8_t sre_length() const;

            byte_array routing_info() const;

            static uint8_t length_static(const byte_array &bytes);

        private:
            byte_array bytes_;
        };

        //////////////////////////////////////////////////////////////////////////
        //
        //////////////////////////////////////////////////////////////////////////
        class TPL_API enhanced_gre_packet : public gre_packet_base {
        HAS_LAYOUT_OF_TYPE(layouts::enhanced_gre_packet_layout)

        public:
            struct fields : gre_packet_base::fields {
                using payload_length = field_descriptor<uint16_t, protocol>;
                using call_id = field_descriptor<uint16_t, payload_length>;
                using seq_number = field_descriptor<uint32_t, call_id>;

                struct ack_number {
                    const static int size = 4;
                };

                static const int header_min_length = call_id::position + call_id::size;
            };

            enhanced_gre_packet();

            explicit enhanced_gre_packet(const byte_array &bytes);

            enhanced_gre_packet(const byte_array &bytes, layout_ptr const &layout);

            explicit enhanced_gre_packet(const placeholders::parent_packet_holder<ip_packet> &parent_ip);

            uint16_t payload_length() const;

            uint16_t call_id() const;

            uint32_t seq_number() const;

            uint32_t ack_number() const;
        };

        PACKET_CAST_DECLARATION(enhanced_gre_packet);
    }
}

REGISTER_TPL_OBJECT(tpl::packets::gre_packet_base, tpl::packets::packet);
REGISTER_TPL_OBJECT(tpl::packets::gre_packet, tpl::packets::gre_packet_base);
REGISTER_TPL_OBJECT(tpl::packets::enhanced_gre_packet, tpl::packets::gre_packet_base);