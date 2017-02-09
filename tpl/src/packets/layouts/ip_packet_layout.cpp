//
// Created by ukheg on 01.10.2016.
//
#include "tpl/packets/layouts/ip_packet_layout.hpp"


namespace tpl {
    namespace packets {
        namespace layouts {
            ip_version ip_packet_layout::version(byte_array const &bytes) const {
                if (bytes.is_empty()) throw std::runtime_error("empty byte array");

                auto version_and_smth_else = bytes[0];
                const uint8_t version_mask = 0xf0;

                auto version_raw_value = (version_and_smth_else & version_mask) >> 4;
                return ip_version(version_raw_value);
            }

	        object_id const& ip_packet_layout::get_packet_id() const
	        {
				return objects_id_repository::id_of<ip_packet>();
	        }
        }
    }
}
