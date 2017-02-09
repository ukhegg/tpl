#pragma once

#include <vector>
#include <list>
#include <map>
#include <tpl/formats/pcap/pcap_file.hpp>
#include <tpl/packets/ip_v4_packet.hpp>
#include <limits>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>
#include "tpl/packets/packet_metainfo_pair.hpp"
#include "tpl/packets/transport/tcp_packet.hpp"
#include "tpl/packets/transport/tcp_packet.hpp"
#include "tpl/packets/transport/udp_packet.hpp"

namespace tpl {
    namespace packets {
        class packet_metainfo;
    }
}


namespace tpl_tests {
    using namespace std;
    using namespace tpl;
    using namespace packets;
    using namespace boost;
    using namespace filesystem;

    class test_helper_impl {
    public:
        test_helper_impl();

        ~test_helper_impl();

        template<class target_packet>
        vector<target_packet> get_packets(const path &p,
                                          std::function<optional<target_packet>(const byte_array &)> bytes_to_packet,
                                          size_t start = 0, size_t count = 0) {
            auto &records(this->file_records_[p]);
            if (records.empty()) {
                this->read_pcap_file(p, records);
            }

            if (start >= records.size()) {
                throw out_of_range("asked record #" + boost::lexical_cast<std::string>(start) +
                                   ",got only " + boost::lexical_cast<std::string>(records.size()));
            }
            if (count == 0) count = numeric_limits<size_t>::max();
            count = min(records.size() - start, count);

            vector<target_packet> result;
            for (auto index = start; index < start + count; ++index) {
                auto r = records[index];
                auto packet_or_nullptr = bytes_to_packet(r.bytes);
                if (packet_or_nullptr == none) continue;
                result.push_back(packet_or_nullptr.get());
            }

            return std::move(result);
        }


        template<class target_packet>
        vector<target_packet> get_packets_unknown_ll(const path &p,
                                                     std::function<optional<target_packet>(const byte_array &,
                                                                                           formats::pcap_link_layer)> bytes_to_packet,
                                                     size_t start = 0, size_t count = 0) {
            auto &records(this->file_records_[p]);
            if (records.empty()) {
                this->read_pcap_file(p, records);
            }

            if (start >= records.size()) throw out_of_range("coder - invalid, bear will programm much better");
            if (count == 0) count = numeric_limits<size_t>::max();
            count = min(records.size() - start, count);

            auto ll = get_link_layer(p);
            vector<target_packet> result;
            for (auto index = start; index < start + count; ++index) {
                auto r = records[index];
                auto packet_or_nullptr = bytes_to_packet(r.bytes, ll);
                if (packet_or_nullptr == none) continue;
                result.push_back(packet_or_nullptr.get());
            }

            return std::move(result);
        }

        template<class target_packet>
        vector<packet_metainfo_pair<target_packet>> get_packets_unknown_ll(
                const path &p,
                std::function<optional<target_packet>(byte_array, packet_metainfo &mi,
                                                      formats::pcap_link_layer)> bytes_to_packet,
                size_t start = 0, size_t count = 0) {
            auto &records(this->file_records_[p]);
            if (records.empty()) {
                this->read_pcap_file(p, records);
            }

            if (start >= records.size()) throw out_of_range("coder - invalid, bear will programm much better");
            if (count == 0) count = numeric_limits<size_t>::max();
            count = min(records.size() - start, count);

            auto ll = get_link_layer(p);
            vector<packet_metainfo_pair<target_packet>> result;
            for (auto index = start; index < start + count; ++index) {
                auto r = records[index];
                packet_metainfo mi;
                mi.set<capture_time_tag>(r.time);
                auto packet_or_nullptr = bytes_to_packet(r.bytes, mi, ll);
                if (packet_or_nullptr == none) continue;
                result.emplace_back(*packet_or_nullptr, mi);
            }

            return std::move(result);
        }

        template<class target_packet>
        target_packet get_packet(const path &p,
                                 std::function<optional<target_packet>(const byte_array &)> bytes_to_packet,
                                 size_t index) {
            auto &&packets = this->get_packets<target_packet>(p, bytes_to_packet, index, 1);
            return packets[0];
        }

        template<class target_packet>
        vector<packet_metainfo_pair<target_packet>> get_packets(const path &p,
                                                                std::function<optional<target_packet>(byte_array,
                                                                                                      packet_metainfo &)> bytes_to_packet,
                                                                size_t start = 0, size_t count = 0) {
            auto &records(this->file_records_[p]);
            if (records.empty()) {
                this->read_pcap_file(p, records);
            }

            if (start >= records.size()) throw out_of_range("coder - invalid, bear will programm much better");
            if (count == 0) count = numeric_limits<size_t>::max();
            count = min(records.size() - start, count);

            vector<packet_metainfo_pair<target_packet>> result;
            for (auto index = start; index < start + count; ++index) {
                packet_metainfo mi;
                auto r = records[index];
                mi.set<capture_time_tag>(r.time);
                auto packet_or_nullptr = bytes_to_packet(r.bytes, mi);
                if (packet_or_nullptr == none) continue;
                result.emplace_back(*packet_or_nullptr, mi);
            }

            return std::move(result);
        }


        template<class target_packet>
        packet_metainfo_pair<target_packet> get_packet(const path &p,
                                                       std::function<optional<target_packet>(byte_array,
                                                                                             packet_metainfo &)> bytes_to_packet,
                                                       size_t index) {
            auto &&packets = this->get_packets<target_packet>(p, bytes_to_packet, index, 1);
            return packets[0];
        }

        path get_test_file(const path &filename) {
            if (filename.is_absolute()) return filename;
            return path(__FILE__).parent_path() / "test_files" / filename;
        }

        byte_array read_content(const path &filepath) {
            auto &content(this->file_contents_[filepath]);
            if (content.is_empty()) {
                auto full_path = get_test_file(filepath);
                content = byte_array(static_cast<size_t>(file_size(full_path)));

                filesystem::ifstream ifs(full_path, ios_base::binary);
                ifs.read(reinterpret_cast<char *>(content.begin()), content.size());
            }
            return content;
        }


        path get_temp_file();

    private:
        map<path, vector<formats::pcap_record>> file_records_;
        map<path, byte_array> file_contents_;
        list <path> temp_files_;

        void read_pcap_file(const path &p,
                            vector<formats::pcap_record> &records) {
            auto reader = formats::pcap_file(get_test_file(p)).open_reader();
            formats::pcap_record record;
            while (reader.read_next(record)) {
                records.push_back(record);
            }
        }

        formats::pcap_link_layer get_link_layer(const path &p) {
            return formats::pcap_file(get_test_file(p)).open_reader().link_layer();
        }
    };


    static test_helper_impl test_helper;


    namespace {
        template<class t_packet, class ... t_packets>
        struct metainfo_gatherer {
            static void gather(packet_metainfo &mi,
                               t_packet const &first_packet,
                               t_packets ... other_packets) {
                metainfo_gatherer<t_packet>::gather(mi, first_packet);
                metainfo_gatherer<t_packets...>::gather(mi, other_packets...);
            }
        };

        template<>
        struct metainfo_gatherer<ip_packet> {
            static void gather(packet_metainfo &mi, ip_packet const &first_packet) {
                mi.set<source_ip_tag>(first_packet.source_address());
                mi.set<destination_ip_tag>(first_packet.destination_address());
            }
        };

        template<>
        struct metainfo_gatherer<ethernet_packet> {
            static void gather(packet_metainfo &mi, ethernet_packet const &first_packet) {
                mi.set<source_mac_tag>(first_packet.source_mac());
                mi.set<destination_mac_tag>(first_packet.destination_mac());
            }
        };

        template<>
        struct metainfo_gatherer<transport_packet> {
            static void gather(packet_metainfo &mi, transport_packet const &first_packet) {
                mi.set<transport_ports_tag>(first_packet.ports());
            }
        };

        template<>
        struct metainfo_gatherer<tcp_packet> {
            static void gather(packet_metainfo &mi, tcp_packet const &first_packet) {
                mi.set<transport_ports_tag>(first_packet.ports());
            }
        };

        template<>
        struct metainfo_gatherer<udp_packet> {
            static void gather(packet_metainfo &mi, udp_packet const &first_packet) {
                mi.set<transport_ports_tag>(first_packet.ports());
            }
        };

        template<class t_packet>
        struct metainfo_gatherer<t_packet> {
            static void gather(packet_metainfo &mi, t_packet const &first_packet) {
                throw std::runtime_error("not implemented");
            }
        };


        template<>
        struct metainfo_gatherer<void> {
            static void gather(packet_metainfo &mi) {}
        };
    }

    template<class ... t_packets>
    void gather_metainfo(packet_metainfo &mi, t_packets ... packets) {
        metainfo_gatherer<t_packets...>::gather(mi, packets ...);
    }
}
