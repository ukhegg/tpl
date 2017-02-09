#pragma once

#include <stdint.h>

namespace tpl {
    namespace formats {
        struct pcap_version {
            pcap_version() {}

            explicit pcap_version(uint32_t v) : raw_version(v) {}

            pcap_version(uint16_t v_major, uint16_t v_minor)
                    : version(v_major, v_minor) {}

            struct version_ {
                version_(uint16_t v_major, uint16_t v_minor)
                        : version_major(v_major),
                          version_minor(v_minor) {}

                uint16_t version_major;
                uint16_t version_minor;
            };

            union {
                version_ version;
                uint32_t raw_version;
            };
        };
    }
}

