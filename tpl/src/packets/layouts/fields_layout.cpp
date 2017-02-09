#include "tpl/packets/layouts/fields_layout.hpp"
#include "tpl/packets/packet.hpp"
namespace tpl
{
	namespace packets
	{
		namespace layouts
		{
			fields_layout::~fields_layout()
			{
			} 

			object_id const& fields_layout::packet_id() const
			{
				if(this->packet_id_ == boost::none)
				{
					this->packet_id_ = this->get_packet_id();
				}
				return this->packet_id_.value();
			}

			const std::shared_ptr<void_packet_layout> void_packet_layout::instance
			= std::make_shared<void_packet_layout>();


			byte_array void_packet_layout::header_bytes(const byte_array& bytes) const
			{
				throw std::runtime_error("should never be called");
			}

			byte_array void_packet_layout::payload_bytes(const byte_array& bytes) const
			{
				throw std::runtime_error("should never be called");
			}

			object_id const& void_packet_layout::get_packet_id() const
			{
				return objects_id_repository::id_of<data_only_packet>();
			}
		}
	}
}
