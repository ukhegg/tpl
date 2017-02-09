#pragma once

#include "tpl/packets/packet.hpp"
#include <boost/type_traits/is_base_and_derived.hpp>

namespace tpl {
    namespace packets {
        namespace placeholders {
            template<class t_packet>
            class parent_packet_holder {
            public:
                explicit parent_packet_holder(const t_packet &packet_arg) : packet(packet_arg) {}

                const t_packet &packet;

                template<class t_packet_base>
                operator parent_packet_holder<t_packet_base>() {
                    static_assert(boost::is_base_and_derived<t_packet_base, t_packet>::value,
                                  "invalid template parameter-it must be base of t_packet");
                    return parent_packet_holder<t_packet_base>(this->packet);
                }
            };

            namespace detail {
                class parent_packet_holder_producer {
                public:
                    template<class t_packet>
                    parent_packet_holder<t_packet> operator=(const t_packet &some_packet) {
                        return parent_packet_holder<t_packet>(some_packet);
                    }
                };
            }


            static detail::parent_packet_holder_producer parent_packet;
        }
    }
}

