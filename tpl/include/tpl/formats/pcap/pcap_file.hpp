#pragma once

#include "tpl/tpl.hpp"
#include "tpl/formats/pcap/pcap_istream.hpp"
#include "tpl/formats/pcap/pcap_ostream.hpp"
#include "tpl/formats/pcap/pcap_records_container.hpp"
#include <boost/filesystem/path.hpp>

namespace tpl {
    namespace formats {
        class TPL_API pcap_file {
        public:
            pcap_file();

            explicit pcap_file(const boost::filesystem::path &p);

            ~pcap_file();

            static pcap_file create_empty(const boost::filesystem::path &p,
                                          pcap_link_layer ll = pcap_link_layer::raw);

            static pcap_file create_if_not_exists(const boost::filesystem::path &p,
                                                  pcap_link_layer ll = pcap_link_layer::raw);

            pcap_istream open_reader() const;

            pcap_ostream open_writer(pcap_link_layer ll = pcap_link_layer::raw) const;

            pcap_records_container records() const;

            bool operator=(const pcap_file &file) const {
                return this->path_ == file.path_;
            }

            bool operator=(const boost::filesystem::path &path) const {
                return this->path_ == path;
            }

            operator boost::filesystem::path() const {
                return this->path_;
            }

        private:
            boost::filesystem::path path_;
        };
    }
}

