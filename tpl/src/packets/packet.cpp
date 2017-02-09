//
// Created by ukheg on 26.09.2016.
//

#include "tpl/packets/packet.hpp"

namespace tpl {
    namespace packets {
        packet::packet() {}

		packet::packet(byte_array const& bytes, field_layout_ptr const& layout)
			: bytes_(bytes), layout_(layout)
	    {
			if(layout == nullptr)
			{
				throw std::invalid_argument("layout should not be null");
			}
	    }

	    packet::packet(placeholders::parent_packet_holder<packet> const& parent) 
			: bytes_(parent.packet.payload_bytes()){}

        byte_array const &packet::bytes() const {
            return this->bytes_;
        }

	    bool packet::operator==(packet const& rhv) const
	    {
		    return this->layout_ == rhv.layout_ &&
			    this->bytes_ == rhv.bytes_;
	    }

	    bool packet::operator!=(packet const& rhv) const
	    {
		    return !(*this == rhv);
	    }

	    object_id const& packet::this_packet_id() const
	    {
			if(this->layout_ == nullptr)
			{
				throw std::runtime_error("packet layout is null");
			}
			return this->layout_->packet_id();
	    }

	    data_only_packet::data_only_packet()
	    {
	    }

	    data_only_packet::data_only_packet(byte_array const& bytes): packet(bytes, layout_type::instance)
	    {
	    }

	    byte_array packet::header_bytes() const {
			if (this->layout_ == nullptr)
			{
				throw std::runtime_error("packet layout is null");
			}
            return this->layout_->header_bytes(this->bytes_);
        }

        byte_array packet::payload_bytes() const {
			if (this->layout_ == nullptr)
			{
				throw std::runtime_error("packet layout is null");
			}
            return this->layout_->payload_bytes(this->bytes_);
        }

        bad_cast_exception::bad_cast_exception() : runtime_error("invalid cast somewhere..."){}

		bad_cast_exception::bad_cast_exception(const char *message) : runtime_error(message) {}
	}
};