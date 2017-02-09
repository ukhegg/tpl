#include "tpl/formats/pcap/pcap_file.hpp"
#include "tpl/formats/pcap/detail/pcap_file_header.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <sstream>

using namespace std;
using namespace boost::filesystem;

namespace tpl {
    namespace formats {
        pcap_file::pcap_file() {}

        pcap_file::pcap_file(const path &p)
                : path_(p) {}

        pcap_file::~pcap_file() {}

        pcap_file pcap_file::create_empty(const path &p, pcap_link_layer ll /*= pcap_link_layer::raw*/) {
            boost::filesystem::ofstream ofs(p, ios_base::binary);


            auto file_header = detail::pcap_file_header::default_header;
            file_header.network = ll;

            ofs.write(reinterpret_cast<char *>(&file_header), sizeof(file_header));
            ofs.flush();
            ofs.close();

            return pcap_file(p);
        }

        pcap_istream pcap_file::open_reader() const {
            auto ifs = new boost::filesystem::ifstream(this->path_, ios_base::binary);
            if (!ifs->is_open()) {
                delete ifs;
                throw invalid_argument("can't open file " + this->path_.generic_string() + " for reading");
            }

            detail::pcap_file_header file_header;
            file_header << *ifs;
            if (file_header.magic_number != detail::pcap_file_header::magix_number_value) {
                ifs->close();
                delete ifs;

                throw invalid_argument("invalid_magic_number");
            }

            return pcap_istream(ifs, file_header.network);
        }

        pcap_ostream pcap_file::open_writer(pcap_link_layer ll /*= pcap_link_layer::raw*/) const {
            if (!exists(this->path_)) create_empty(this->path_, ll);
            else {
                auto reader = this->open_reader();
                if (reader.link_layer() != ll) {
                    ostringstream oss;
                    oss << "incompatible link layer in file " << this->path_
                        << ":expected " << static_cast<int>(ll) << " actual " << static_cast<int>(reader.link_layer());
                    throw invalid_argument(oss.str());
                }
            }

            auto ofs = new boost::filesystem::ofstream(this->path_, ios_base::binary | ios_base::app);
            if (!ofs->is_open()) {
                delete ofs;
                throw invalid_argument("can't open file " + this->path_.generic_string() + " for writing");
            }
            return pcap_ostream(ofs);
        }

        pcap_records_container pcap_file::records() const {
            return pcap_records_container(this->open_reader());
        }

        pcap_file pcap_file::create_if_not_exists(const path &p,
                                                  pcap_link_layer ll /*= pcap_link_layer::raw*/) {
            if (!exists(p)) create_empty(p, ll);

            pcap_file file(p);
            file.open_writer(ll);
            return file;
        }
    }
}

