//
// Created by ukheg on 28.09.2016.
//

#ifndef PLAIN_PACKETS_PACKET_FIELDS_HPP
#define PLAIN_PACKETS_PACKET_FIELDS_HPP

namespace tpl{
    namespace packets{
        namespace details{
            template<class t_field>
            struct little_endian {
            private:
                t_field dummy_;//for sizeof(little_endian<T>) == sizeof(T)
            };

            template<class t_field>
            struct big_endian {
            private:
                t_field dummy_;//for sizeof(big_endian<T>) == sizeof(T)
            };
        }
    }
}
#endif //PLAIN_PACKETS_PACKET_FIELDS_HPP
