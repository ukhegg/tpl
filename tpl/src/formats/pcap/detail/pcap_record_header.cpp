#include "tpl/formats/pcap/detail/pcap_record_header.h"
#include <istream>

namespace tpl {
    namespace formats {
        namespace detail {
            std::istream &pcap_record_header::operator<<(std::istream &is) {
                is.read(reinterpret_cast<char *>(this), sizeof(pcap_record_header));
                return is;
            }
        }
    }
}

