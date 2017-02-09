# tpl

Main goal of this library is to simplify common problems while processing network traffic:
* getting network packets fields
- processing packet stacks
- reading/writing captured traffic from files in common formats(e.g. pcap)
- building a sequence of packet handlers to tranform packets,e.g. defragment ipv4->extract transport layer packets->detect protocol->write to disk
- do this with minimal memory coping


In basic of this library lies d_array. Its code is based on boost shared_array. Adding size field makes it possible to create subviews(prefix,suffix,middle) pointing the same memory region.When last of views is destroed,memory is freed. 


