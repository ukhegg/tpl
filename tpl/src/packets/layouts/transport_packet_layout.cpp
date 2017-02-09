//
// Created by ukheg on 05.10.2016.
//

#include "tpl/packets/layouts/transport_packet_layout.hpp"
#include "tpl/packets/transport/transport_packet.hpp"


namespace tpl {
    namespace packets {
        namespace layouts {
	        object_id const& transport_packet_layout::get_packet_id() const
	        {
				return objects_id_repository::id_of<transport_packet>();
	        }
        }
    }
}
