//
// Created by ukheg on 26.09.2016.
//

#ifndef TPL_LIB_ETHERNET_PACKET_HPP
#define TPL_LIB_ETHERNET_PACKET_HPP

#include "tpl/packets/packet.hpp"
#include "tpl/utils/mac_address.hpp"
#include "tpl/packets/data_link/ethernet_packet.hpp"


namespace tpl {
    namespace packets {
        namespace layouts { class ethernet_packet_layout; }

        enum class ethernet_payload_types : uint16_t {
            ip = 0x0800,
            arp = 0x0806,
            ipx = 0x8137,
            ip_v6 = 0x86dd
            /*@    0000-05DC	IEEE802.3 Length Field (0.:1500.)
+    0101-01FF	Experimental
 0200	Xerox PUP (conflicts with 802.3 Length Field range) (see 0A00)
 0201	Xerox PUP Address Translation (conflicts ...) (see 0A01)
 0400	Nixdorf (conflicts with 802.3 Length Field)
+*   0600	Xerox NS IDP
 0601	XNS Address Translation (3Mb only)
+*   0800	DOD Internet Protocol (IP)
+    0801	X.75 Internet
+    0802	NBS Internet
+    0803	ECMA Internet
+    0804	CHAOSnet
+    0805	X.25 Level 3
+*   0806	Address Resolution Protocol (ARP) (for IP and for CHAOS)
 0807	XNS Compatibility
 081C	Symbolics Private
+    0888-088A	Xyplex
 0900	Ungermann-Bass network debugger
 0A00	Xerox IEEE802.3 PUP
 0A01	Xerox IEEE802.3 PUP Address Translation
 0BAD	Banyan Systems
 0BAF	Banyon VINES Echo
 1000	Berkeley Trailer negotiation
 1001-100F	Berkeley Trailer encapsulation for IP
 1234	DCA - Multicast
*    1600	VALID system protocol
 1989	Artificial Horizons ("Aviator" dogfight simulator [on Sun])
 1995	Datapoint Corporation (RCL lan protocol)
 3C00	3Com NBP virtual circuit datagram (like XNS SPP) not registered
 3C01	3Com NBP System control datagram not registered
 3C02	3Com NBP Connect request (virtual cct) not registered
 3C03	3Com NBP Connect repsonse not registered
 3C04	3Com NBP Connect complete not registered
 3C05	3Com NBP Close request (virtual cct) not registered
 3C06	3Com NBP Close response not registered
 3C07	3Com NBP Datagram (like XNS IDP) not registered
 3C08	3Com NBP Datagram broadcast not registered
 3C09	3Com NBP Claim NetBIOS name not registered
 3C0A	3Com NBP Delete Netbios name not registered
 3C0B	3Com NBP Remote adaptor status request not registered
 3C0C	3Com NBP Remote adaptor response not registered
 3C0D	3Com NBP Reset not registered
 4242	PCS Basic Block Protocol
 424C       Information Modes Little Big LAN diagnostic
 4321	THD - Diddle
 4C42       Information Modes Little Big LAN
%    5208	BBN Simnet Private
 6000	DEC unassigned, experimental
 6001	DEC Maintenance Operation Protocol (MOP) Dump/Load Assistance
 6002	DEC Maintenance Operation Protocol (MOP) Remote Console
 6003	DECNET Phase IV, DNA Routing
 6004	DEC Local Area Transport (LAT)
 6005	DEC diagnostic protocol (at interface initialization?)
 6006	DEC customer protocol
 6007	DEC Local Area VAX Cluster (LAVC), System Communication Architecture (SCA)
 6008	DEC AMBER
 6009	DEC MUMPS
+    6010-6014	3Com Corporation
 7000	Ungermann-Bass download
 7001	Ungermann-Bass NIUs
 7002	Ungermann-Bass diagnostic/loopback
 7003	Ungermann-Bass ??? (NMC to/from UB Bridge)
 7005	Ungermann-Bass Bridge Spanning Tree
 7007	OS/9 Microware
 7009	OS/9 Net?
+    7020-7029	LRT (England) (now Sintrom)
 7030	Racal-Interlan
 7031	Prime NTS (Network Terminal Service)
 7034	Cabletron
 8003	Cronus VLN
 8004	Cronus Direct
 8005	HP Probe protocol
+    8006	Nestar
+    8008	AT&T/Stanford Univ.	Local use
 8010	Excelan
+    8013	Silicon Graphics diagnostic
+    8014	Silicon Graphics network games
+    8015	Silicon Graphics reserved
+    8016	Silicon Graphics XNS NameServer, bounce server
+    8019	Apollo DOMAIN
+    802E	Tymshare
+    802F	Tigan, Inc.
+    8035	Reverse Address Resolution Protocol (RARP)
+    8036	Aeonic Systems
 8037	IPX (Novell Netware?)
 8038	DEC LanBridge Management
 8039	DEC DSM/DDP
 803A	DEC Argonaut Console
 803B	DEC VAXELN
 803C	DEC DNS Naming Service
 803D	DEC Ethernet CSMA/CD Encryption Protocol
 803E	DEC Distributed Time Service
 803F	DEC LAN Traffic Monitor Protocol
 8040	DEC PATHWORKS DECnet NETBIOS Emulation
 8041	DEC Local Area System Transport
 8042	DEC unassigned
+    8044	Planning Research Corp.
+    8046	AT&T
+    8047	AT&T
 8048	DEC Availability Manager for Distributed Systems DECamds (but someone at DEC says not)
+    8049	ExperData
+    805B	VMTP (Versatile Message Transaction Protocol, RFC-1045) (Stanford) [was Stanford V Kernel, experimental]
+    805C	Stanford V Kernel, version 6.0
+    805D	Evans & Sutherland
+    8060	Little Machines
+    8062	Counterpoint Computers
+    8065	University of Mass. at Amherst
+    8066	University of Mass. at Amherst
+    8067	Veeco Integrated Automation
+    8068	General Dynamics
+    8069	AT&T
+    806A	Autophon
+    806C	ComDesign
+    806D	Compugraphic Corporation
+    806E-8077	Landmark Graphics Corporation
+    807A	Matra
+    807B	Dansk Data Elektronik
+    807C	Merit Internodal (or Univ of Michigan?)
+    807D-807F	Vitalink Communications
+    8080	Vitalink TransLAN III Management
+    8081-8083	Counterpoint Computers
 8088-808A	Xyplex
+    809B	EtherTalk (AppleTalk over Ethernet)
+    809C-809E	Datability
+    809F	Spider Systems Ltd.
+    80A3	Nixdorf Computers
+    80A4-80B3	Siemens Gammasonics Inc.
+    80C0-80C3	DCA (Digital Comm. Assoc.) Data Exchange Cluster
+    80C6	Pacer Software
+    80C7	Applitek Corporation
+    80C8-80CC	Intergraph Corporation
+    80CD-80CE	Harris Corporation
+    80CF-80D2	Taylor Instrument
+    80D3-80D4	Rosemount Corporation
 80D5	IBM SNA Services over Ethernet
+    80DD	Varian Associates
+    80DE-80DF	TRFS (Integrated Solutions Transparent Remote File System)
+    80E0-80E3	Allen-Bradley
+    80E4-80F0	Datability
+    80F2	Retix
+    80F3	AppleTalk Address Resolution Protocol (AARP)
+    80F4-80F5	Kinetics
+    80F7	Apollo Computer
+    80FF-8101	Wellfleet Communications
+    8102	Wellfleet; BOFL (Breath OF Life) pkts [every 5-10 secs.]
+    8103	Wellfleet Communications
 8107-8109	Symbolics Private
 812B	Talaris
+    8130	Waterloo Microsystems Inc.
+    8131	VG Laboratory Systems
+    8137	Novell (old) NetWare IPX (ECONFIG E option)
+    8138	Novell, Inc.
+    8139-813D	KTI
 813F	M/MUMPS data sharing
 8145	Vrije Universiteit (NL)		Amoeba 4 RPC (obsolete)
 8146	Vrije Universiteit (NL)		FLIP (Fast Local Internet Protocol)
 8147	Vrije Universiteit (NL)		[reserved]
 814C	SNMP over Ethernet (see RFC1089)
 814F	Technically Elite Concepts	Network Professor
 8191	PowerLAN			NetBIOS/NetBEUI (PC)
 817D	XTP
 81D6	Artisoft Lantastic
 81D7	Artisoft Lantastic
 8203-8205	QNX Software Systems Ltd.
 8390	Accton Technologies (unregistered)
 852B	Talaris multicast
 8582	Kalpana
 86DD       IP version 6
 8739	Control Technology Inc.		RDP Without IP
 873A	Control Technology Inc.		Mcast Industrial Ctrl Proto.
 873B	Control Technology Inc.		Proprietary
 873C	Control Technology Inc.		Proprietary
 8820	Hitachi Cable (Optoelectronic Systems Laboratory)
 8856       Axis Communications AB		proprietary bootstrap/config
 8888	HP LanProbe test?
+    9000	Loopback (Configuration Test Protocol)
 9001	3Com (Formerly Bridge Communications), XNS Systems Management
 9002	3Com (Formerly Bridge Communications), TCP/IP Systems Management
 9003	3Com (Formerly Bridge Communications), loopback detection
 AAAA	DECNET?		Used by VAX 6220 DEBNI
 FAF5	Sonix Arpeggio
%    FF00	BBN VITAL-LanBridge cache wakeups*/
        };

        class TPL_API ethernet_packet : public packet {
        HAS_LAYOUT_OF_TYPE(layouts::ethernet_packet_layout)

        public:
            struct TPL_API fields {
                typedef field_descriptor<details::byte_sequence<utils::mac_address::mac_address_length>> destination_mac;
                typedef field_descriptor<details::byte_sequence<utils::mac_address::mac_address_length>, destination_mac> source_mac;
                typedef field_descriptor<ethernet_payload_types, source_mac> payload_type;

                enum : size_t {
                    header_length = payload_type::position + payload_type::size
                };
            };

            explicit ethernet_packet(byte_array const &bytes);

            ethernet_packet(byte_array const &bytes, layout_ptr const &layout);

            utils::mac_address source_mac() const;

            utils::mac_address destination_mac() const;

            ethernet_payload_types payload_type() const;
        };

        template<>
        TPL_API ethernet_packet packet::as<ethernet_packet>() const;
    }
}
REGISTER_TPL_OBJECT(tpl::packets::ethernet_packet, tpl::packets::packet);


#endif //TPL_LIB_ETHERNET_PACKET_HPP
