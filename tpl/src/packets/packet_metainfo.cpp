#include "tpl/packets/packet_metainfo.hpp"


namespace tpl {
    namespace packets {
        metainfo_data::metainfo_data() {}

        metainfo_data::metainfo_data(int flag) {}

        metainfo_data::~metainfo_data() {}

        packet_metainfo::packet_metainfo()
                : data_(std::make_shared<metainfo_data>()) {}


        packet_metainfo::~packet_metainfo() {}
    }
}

