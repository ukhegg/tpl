//
// Created by ukhegg on 17.10.2016.
//

#ifndef PLAIN_PACKETS_TRANSPORT_TYPE_HPP
#define PLAIN_PACKETS_TRANSPORT_TYPE_HPP
#include <iostream>

namespace tpl {
    namespace packets {
        enum class transport_type {
            tcp,
            udp
        };

		template<class t_char>
        std::basic_ostream<t_char> &operator<<(std::basic_ostream<t_char> &os, transport_type type)
        {
            switch(type){
                case transport_type::udp:
                    os << "udp";
                    break;
                case transport_type::tcp:
                    os << "tcp";
                    break;
                default:
                    os << "unknown transport type(" << static_cast<int>(type) << ")";
                    break;
            }
            return os;
        }
    }
}


#endif //PLAIN_PACKETS_TRANSPORT_TYPE_HPP
