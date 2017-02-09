#pragma once

#include "tpl/utils/d_array.hpp"
#include <boost/date_time/posix_time/ptime.hpp>

namespace tpl {
    namespace formats {
        struct pcap_record {
            pcap_record() {}

            pcap_record(const byte_array &b, const boost::posix_time::ptime &t)
                    : bytes(b), time(t) {}

            byte_array bytes;
            boost::posix_time::ptime time;
        };
    }
}

