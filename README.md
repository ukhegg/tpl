# tpl

Main goal of this library is to simplify common problems while processing network traffic:
* getting network packets fields
- processing packet stacks
- reading/writing captured traffic from files in common formats(e.g. pcap)
- building a sequence of packet handlers to tranform packets,e.g. 
  * defragment ipv4 ->
  * extract transport layer packets ->
  * detect protocol ->
  * write to disk
- do this with minimal memory coping

To minimize memory coping i use d_array.The name comes from dlang arrays,where you can take subsequences without coping memory.I took boost shared_array and adopted it to be able to track its size and yake prefix,suffix,middle. 
Network packet has header,possibly has a payload. Each packet holds its bytes and a pointer to layout. Layout incapsulates the logic to get various packet's fields,basically header and payload.Also it holds packet id,so we can get packet type identifier given only reference to packet base class.

To simplify implementing packets layout,field_descriptor<t_field_type, t_previous_field> class is used. Based on it's template parameters, it defines(possibly) field's __size__ and __position__ enum values and generates getters and setters. __Position__ enum value is defined if previous field is __void__ or has __position__ and __size__ defined. __Size__ enum value is defined for integer,enum and byte fields with fixed size.  Currently fields types:
* raw byte fields
* enum fields
* integer fields

Usage(taken from ipv6 packet):
```cpp
struct fields {
                using version_traffic_and_flow_label = field_descriptor<uint32_t>;
                using payload_length = field_descriptor<uint16_t, version_traffic_and_flow_label>;
                using next_header = field_descriptor<ip_payload_protocols , payload_length>;
                using hop_limit = field_descriptor<uint8_t, next_header>;
                using source_addr = field_descriptor<details::byte_sequence<16>, hop_limit>;
                using destination_addr = field_descriptor<details::byte_sequence<16>, source_addr>;

                static const int header_length = destination_addr::position + destination_addr::size;
                static const int extension_headers_position = header_length;
            };
```
Here:
* __version_traffic_and_flow_label__ is uint32_t field at offset 0
* __payload_length__ is uint16_t fields at offset sizeof(__version_traffic_and_flow_label__)
* __next_header__ is enum field at offset __version_traffic_and_flow_label__::position + __version_traffic_and_flow_label__::size 
* and so on

#####Currently implemented:
* formats:
  * pcap - pcap file/stream readers/writer
* utils:
  * bit_converter - functions to convert network-byte ordered fields to uint<N>_t, can be used with d_array<uint8_t>
  * d_array
  * data_size - class for representing size of various data-files,captured traffic e.t.c. Includes arithmetic and io
  * ip_address_range
  * ip_direction
  * ip_range_set - set of ip_address_ranges, having O(logN) complexity to check if it contains given ip or not(N is ranges count,not ips)
  * mac_address
  * transport_conversation_id - tuple (ip_from,ip_to,port_from,port_to,transport_type)
* packets:
  * ethernet
  * ip v4/v6
  * gre packet
  * udp&tcp
  
  
