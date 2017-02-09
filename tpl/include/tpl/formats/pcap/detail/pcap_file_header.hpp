#pragma once

#include "tpl/formats/pcap/pcap_version.hpp"
#include "tpl/formats/pcap/pcap_link_layer.h"
#include <istream>
#include <ostream>

namespace tpl {
    namespace formats {
        namespace detail {
            struct pcap_file_header {
                static const uint32_t magix_number_value = 0xa1b2c3d4;
                static const uint32_t max_snapshot_length = 0xffff;
                static const pcap_file_header default_header;

                uint32_t magic_number;
                pcap_version version;
                uint32_t this_zone;
                uint32_t sig_flags;
                uint32_t snapshot_length;
                pcap_link_layer network;

                std::istream &operator<<(std::istream &is);

            private:
                static pcap_file_header create_default();
            };

            std::ostream &operator<<(std::ostream &os, const pcap_file_header &file_header);
        }
    }
}

