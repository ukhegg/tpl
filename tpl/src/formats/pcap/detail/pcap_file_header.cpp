#include "tpl/formats/pcap/detail/pcap_file_header.hpp"

namespace tpl {
    namespace formats {
        namespace detail {
            std::istream &pcap_file_header::operator<<(std::istream &is) {
                is.read(reinterpret_cast<char *>(this), sizeof(pcap_file_header));
                return is;
            }

            pcap_file_header pcap_file_header::create_default() {
                pcap_file_header header;
                header.magic_number = pcap_file_header::magix_number_value;
                header.snapshot_length = pcap_file_header::max_snapshot_length;
                header.sig_flags = 0;
                header.this_zone = 0;
                header.version = pcap_version(4, 2);
                return header;
            }

            const pcap_file_header pcap_file_header::default_header = pcap_file_header::create_default();

            std::ostream &operator<<(std::ostream &os, const pcap_file_header &file_header) {
                os.write(reinterpret_cast<const char *>(&file_header), sizeof(pcap_file_header));
                return os;
            }
        }
    }
}

