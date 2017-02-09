//
// Created by ukheg on 27.09.2016.
//

#ifndef PLAIN_PACKETS_IP_V4_FLAGS_HPP
#define PLAIN_PACKETS_IP_V4_FLAGS_HPP


namespace tpl{
    namespace packets{
        struct ip_v4_flags {
            explicit ip_v4_flags(uint8_t value) {
                static_assert(sizeof(ip_v4_flags) == sizeof(uint8_t), "invalid ip_flags alignment");
                reinterpret_cast<uint8_t *>(this)[0] = value;
            }

            uint8_t staffing : 5;
            bool more_fragments : 1;
            bool do_not_fragment : 1;
            bool reserved_bit : 1;
        };
    }
}
#endif //PLAIN_PACKETS_IP_V4_FLAGS_HPP
