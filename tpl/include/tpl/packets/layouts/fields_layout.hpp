//
// Created by ukheg on 26.09.2016.
//

#ifndef TPL_LIB_FIELDS_LAYOUT_HPP
#define TPL_LIB_FIELDS_LAYOUT_HPP

#include "tpl/utils/d_array.hpp"
#include "tpl/object_id.hpp"

namespace tpl
{
	namespace packets
	{
		namespace layouts
		{
			class fields_layout
			{
			public:
				virtual ~fields_layout();

				virtual byte_array header_bytes(const byte_array& bytes) const = 0;

				virtual byte_array payload_bytes(const byte_array& bytes) const = 0;

				object_id const& packet_id() const;
			protected:
				virtual object_id const& get_packet_id() const = 0;
			private:
				mutable boost::optional<object_id> packet_id_;
			};

			class void_packet_layout : public fields_layout
			{
			public:
				static const std::shared_ptr<void_packet_layout> instance;

				byte_array header_bytes(const byte_array& bytes) const override;

				byte_array payload_bytes(const byte_array& bytes) const override;
			protected:
				object_id const& get_packet_id() const override;
			};
		}
	}
}


#endif //TPL_LIB_FIELDS_LAYOUT_HPP
