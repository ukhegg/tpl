#include "tpl/packets/gre_packet.hpp"
#include "tpl/packets/layouts/gre_packet_layout.hpp"

namespace tpl {
    namespace packets {
        //////////////////////////////////////////////////////////////////////////
        // base gre
        //////////////////////////////////////////////////////////////////////////
        std::shared_ptr<layouts::fields_layout> select_layout(byte_array const &bytes) {
            auto f = gre_flags(gre_packet_base::fields::flags::get(bytes));
            switch (f.version()) {
                case gre_version::gre:
                    return layouts::gre_packet_layout_base::select_layout(f);
                case gre_version::enhanced_gre:
                    return layouts::enhanced_gre_packet_layout::instance;
                default:
                    return nullptr;
                    break;
            }
        }

        gre_packet_base::gre_packet_base() {}

        gre_packet_base::gre_packet_base(const byte_array &bytes)
                : packet(bytes, select_layout(bytes)) {}

	    gre_packet_base::gre_packet_base(const byte_array& bytes, layout_ptr const& layout)
			: packet(bytes, layout)
	    {
	    }

	    gre_packet_base::gre_packet_base(const placeholders::parent_packet_holder<packet> &parent)
                : gre_packet_base(parent.packet.payload_bytes()) {
        }

        gre_packet_base::~gre_packet_base() {}

        gre_flags gre_packet_base::flags() const {
            return gre_flags(fields::flags::get(this->bytes_));
        }

        gre_payload_protocol gre_packet_base::payload_protocol() const {
            return fields::protocol::get(this->bytes_);
        }

		PACKET_CAST_DEFINITION(gre_packet_base);

        //////////////////////////////////////////////////////////////////////////
        // gre version 0
        //////////////////////////////////////////////////////////////////////////
        gre_packet::gre_packet() {}

        gre_packet::gre_packet(const byte_array &bytes)
                : gre_packet_base(bytes) {}

	    gre_packet::gre_packet(const byte_array& bytes, layout_ptr const& layout)
			: gre_packet_base(bytes, layout)
	    {
	    }

	    gre_packet::gre_packet(const placeholders::parent_packet_holder<packet> &parent_packet)
                : gre_packet_base(parent_packet) {}

        gre_packet::~gre_packet() {}

        boost::optional<uint16_t> gre_packet::checksum() const {
            return fields::checksum::get(this->bytes_);
        }

        boost::optional<uint16_t> gre_packet::offset() const {
            return fields::offset::get(this->bytes_);
        }

        boost::optional<uint32_t> gre_packet::key() const {
            return this->layout().key(this->bytes_);
        }

        boost::optional<uint32_t> gre_packet::seq_number() const {
            return this->layout().seq_number(this->bytes_);
        }

        gre_sre::gre_sre() {}

        gre_sre::gre_sre(const byte_array &bytes) : bytes_(bytes) {}

        uint16_t gre_sre::address_family() const {
            return utils::ntoh<uint16_t>(this->bytes_, fields::address_family::position);
        }

        uint8_t gre_sre::sre_offset() const {
            return this->bytes_[fields::sre_offset::position];
        }

        uint8_t gre_sre::sre_length() const {
            return this->bytes_[fields::sre_length::position];
        }

        byte_array gre_sre::routing_info() const {
            return this->bytes_.suffix(fields::routing_info::position);
        }

        uint8_t gre_sre::length_static(const byte_array &bytes) {
            return bytes[fields::sre_length::position];
        }

		PACKET_CAST_DEFINITION(gre_packet);
        //////////////////////////////////////////////////////////////////////////
        // gre version 1 (enchansed)
        //////////////////////////////////////////////////////////////////////////

        enhanced_gre_packet::enhanced_gre_packet() {}

        enhanced_gre_packet::enhanced_gre_packet(const byte_array &bytes)
                : gre_packet_base(bytes) {}

	    enhanced_gre_packet::enhanced_gre_packet(const byte_array& bytes, layout_ptr const& layout)
			:gre_packet_base(bytes, layout)
	    {
	    }

	    enhanced_gre_packet::enhanced_gre_packet(const placeholders::parent_packet_holder<ip_packet> &parent_ip)
                : gre_packet_base(placeholders::parent_packet_holder<packet>(parent_ip.packet)) {}

        uint16_t enhanced_gre_packet::payload_length() const {
            return this->layout().payload_length(this->bytes_);
        }

        uint16_t enhanced_gre_packet::call_id() const {
            return this->layout().call_id(this->bytes_);
        }

        uint32_t enhanced_gre_packet::seq_number() const {
            return this->layout().seq_number(this->bytes_);
        }

        uint32_t enhanced_gre_packet::ack_number() const {
            return this->layout().ack_number(this->bytes_);
        }

		PACKET_CAST_DEFINITION(enhanced_gre_packet);

        gre_flags::gre_flags(uint16_t value) {
            this->raw_value = value;
            static_assert(sizeof(gre_flags) == sizeof(uint16_t), "invalid gre_flags alignment");
        }

        gre_flags::gre_flags() {

        }
    }
}

