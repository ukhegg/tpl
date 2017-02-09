#pragma once

#include <ostream>
#include <stdint.h>

namespace tpl {
    namespace formats {
        namespace detail {
            struct pcap_record_header {
            public:
                uint32_t timestamp_seconds;
                uint32_t timestamp_milliseconds;
                uint32_t bytes_saved;
                uint32_t packet_length;

                std::istream &operator<<(std::istream &is);
            };
        }
    }
}

