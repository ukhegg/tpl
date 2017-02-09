//
// Created by ukheg on 26.09.2016.
//

#ifndef PLAIN_PACKETS_IP_PACKET_HPP
#define PLAIN_PACKETS_IP_PACKET_HPP

#include "packet.hpp"
#include "tpl/packets/layouts/fields_layout.hpp"
#include <tpl/packets/ip_packet.hpp>
#include <boost/asio/ip/address.hpp>

namespace tpl{
    namespace packets{
        enum class ip_version : uint8_t {
            ip_v4 = 0x04,
            ip_v6 = 0x06
        };

        enum class ip_payload_protocols : uint8_t {
            hopopts = 0,
            /// <summary> internet control message protocol. </summary>
                    icmp = 1,
            /// <summary> internet group management protocol.</summary>
                    igmp = 2,
            /// <summary> ipip tunnels (older ka9q tunnels use 94). </summary>
                    ipip = 4,
            /// <summary> transmission control protocol. </summary>
                    tcp = 6,
            /// <summary> exterior gateway protocol. </summary>
                    egp = 8,
            /// <summary> pup protocol. </summary>
                    pup = 12,
            /// <summary> user datagram protocol. </summary>
                    udp = 17,
            /// <summary> xns idp protocol. </summary>
                    idp = 22,
            /// <summary> so transport protocol class 4. </summary>
                    tp = 29,
            /// <summary> ipv6 header. </summary>
                    ipv6 = 41,
            /// <summary> ipv6 routing header. </summary>
                    routing = 43,
            /// <summary> ipv6 fragmentation header. </summary>
                    fragment = 44,
            /// <summary> reservation protocol. </summary>
                    rsvp = 46,
            /// <summary> general routing encapsulation. </summary>
                    gre = 47,
            /// <summary> encapsulating security payload. </summary>
                    esp = 50,
            /// <summary> authentication header. </summary>
                    ah = 51,
            /// <summary> icmpv6. </summary>
                    icmpv6 = 58,
            /// <summary> ipv6 no next header. </summary>
                    none = 59,
            /// <summary> ipv6 destination options. </summary>
                    dstopts = 60,
            /// <summary> multicast transport protocol. </summary>
                    mtp = 92,
            /// <summary> encapsulation header. </summary>
                    encap = 98,
            /// <summary> protocol independent multicast. </summary>
                    pim = 103,
            /// <summary> compression header protocol. </summary>
                    comp = 108,
            /// <summary> mobility header
                    mobility = 135,
            /// <summary> raw ip packets. </summary>
                    raw = 255
        };

        namespace layouts { class ip_packet_layout; }

        class TPL_API ip_packet : public packet{
            HAS_LAYOUT_OF_TYPE(layouts::ip_packet_layout);
        public:
            struct fields{
                static const uint8_t version_mask = 0xf0;
            };

            ip_packet();

	        explicit ip_packet(byte_array const& bytes);//automatically detect version

			ip_packet(byte_array const& bytes, layout_ptr const& layout);

	        explicit ip_packet(placeholders::parent_packet_holder<packet> parent);

            ip_version version() const;

            boost::asio::ip::address source_address() const;

            boost::asio::ip::address destination_address() const;

            ip_payload_protocols next_header() const;
        };
		PACKET_CAST_DECLARATION(ip_packet);
    }
}

REGISTER_TPL_OBJECT(tpl::packets::ip_packet, tpl::packets::packet);



#endif //PLAIN_PACKETS_IP_PACKET_HPP
