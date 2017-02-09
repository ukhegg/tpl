#pragma once

#include "packet_metainfo.hpp"

namespace tpl {
    namespace packets {
        template<class t_packet>
        struct packet_metainfo_pair {
            packet_metainfo_pair() {}

            packet_metainfo_pair(t_packet const &packet_arg, packet_metainfo const &info)
                    : packet(packet_arg), metainfo(info) {}

            t_packet packet;
            packet_metainfo metainfo;
        };

        template<class t_packet>
        packet_metainfo_pair<t_packet> make_packet_metainfo_pair(t_packet const &p, packet_metainfo const &metainfo) {
            return packet_metainfo_pair<t_packet>(p, metainfo);
        }
    }
}

