#pragma once
#include "tpl/tpl.h"
#include <boost/asio/ip/address.hpp>

namespace tpl {
    namespace utils {
        struct TPL_API transport_conversation_id {
            transport_conversation_id() : src_port(0), dst_port(0) {}

            transport_conversation_id(boost::asio::ip::address const &src_address,
                                      boost::asio::ip::address const &dst_address,
                                      uint16_t src_port_arg,
                                      uint16_t dst_port_arg)
                    : src_address(src_address),
                      dst_address(dst_address),
                      src_port(src_port_arg),
                      dst_port(dst_port_arg) {}


            boost::asio::ip::address src_address;
            boost::asio::ip::address dst_address;
            uint16_t src_port;
            uint16_t dst_port;

			friend TPL_API bool operator<(transport_conversation_id const &lhs, transport_conversation_id const &rhv) {
                if (lhs.src_port != rhv.src_port) return lhs.src_port < rhv.src_port;
                if (lhs.dst_port != rhv.dst_port) return lhs.dst_port < rhv.dst_port;
                if (lhs.src_address != rhv.src_address) return lhs.src_address < rhv.src_address;
                return lhs.dst_address < rhv.dst_address;
            }


			 friend bool TPL_API operator==(transport_conversation_id const &lhs, transport_conversation_id const &rhv) {
                return lhs.src_address == rhv.src_address
                       && lhs.dst_address == rhv.dst_address
                       && lhs.src_port == rhv.src_port
                       && lhs.dst_port == rhv.dst_port;
            }

			 friend TPL_API bool operator!=(transport_conversation_id const &lhs, transport_conversation_id const &rhv) {
                return !(lhs == rhv);
            }
        };
    }
}

