//
// Created by ukheg on 26.09.2016.
//

#ifndef TPL_LIB_PACKET_HPP
#define TPL_LIB_PACKET_HPP

#include "tpl/tpl.hpp"
#include "tpl/utils/d_array.hpp"
#include "tpl/packets/layouts/fields_layout.hpp"
#include "tpl/packets/field_descriptor.hpp"
#include "tpl/packets/placeholders.hpp"
#include "tpl/object_id.hpp"

namespace tpl
{
	namespace packets
	{
		namespace layouts
		{
			
		}

		struct bad_cast_exception : std::runtime_error
		{
			bad_cast_exception();

			explicit bad_cast_exception(const char* message);
		};

		class TPL_API packet
		{
		public:
			using field_layout_ptr = std::shared_ptr<layouts::fields_layout>;

			packet();

			packet(byte_array const& bytes, field_layout_ptr const& layout);

			explicit packet(placeholders::parent_packet_holder<packet> const& parent);

			byte_array header_bytes() const;

			byte_array payload_bytes() const;

			byte_array const& bytes() const;

			bool operator==(packet const& rhv) const;

			bool operator!=(packet const& rhv) const;

			template <class t_derived>
			t_derived as() const;

			template <class t_derived>
			bool is() const;

			object_id const& this_packet_id() const;
		protected:
			byte_array bytes_;
			field_layout_ptr layout_;
		};

#define HAS_LAYOUT_OF_TYPE(TLayout) public: using layout_type = TLayout;\
											using layout_ptr = std::shared_ptr<layout_type>;\
									private: layout_type& layout() const { return reinterpret_cast<layout_type&>(*this->layout_); }

#define PACKET_CAST_DECLARATION(TargetPacket) template<> TPL_API TargetPacket packet::as<TargetPacket>() const;\
											  template<> TPL_API bool packet::is<TargetPacket>() const;

#define PACKET_CAST_DEFINITION(TargetPacket) \
    	template<> TargetPacket packet::as() const {\
			auto layout = std::dynamic_pointer_cast<TargetPacket::layout_type>(this->layout_);\
    		if(layout == nullptr) throw bad_cast_exception((std::string("invalid cast to ") + typeid(TargetPacket).name()).c_str());\
    		return TargetPacket(this->bytes_, layout);}\
		template<> bool packet::is<TargetPacket>() const { return std::dynamic_pointer_cast<TargetPacket::layout_type>(this->layout_) != nullptr; }

		class TPL_API data_only_packet : public packet
		{
			HAS_LAYOUT_OF_TYPE(layouts::void_packet_layout);
		public:
			data_only_packet();

			explicit data_only_packet(byte_array const& bytes);
		};
		
	}
}
REGISTER_TPL_OBJECT(tpl::packets::packet, tpl::root_object);
REGISTER_TPL_OBJECT(tpl::packets::data_only_packet, tpl::packets::packet);

#endif //TPL_LIB_PACKET_HPP
