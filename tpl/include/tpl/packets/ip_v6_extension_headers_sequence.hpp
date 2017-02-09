//
// Created by ukheg on 02.10.2016.
//

#ifndef PLAIN_PACKETS_EP_V6_EXTENSION_HEADERS_SEQUENCE_HPP
#define PLAIN_PACKETS_EP_V6_EXTENSION_HEADERS_SEQUENCE_HPP

#include "tpl/packets/ip_v6_extension_header.hpp"

namespace tpl {
    namespace packets {
        namespace iterators {
            class TPL_API ipv6_ext_headers_iterator : public std::iterator<std::forward_iterator_tag, ip_v6_extension_header> {
            public:
                ipv6_ext_headers_iterator();

                ipv6_ext_headers_iterator(byte_array const &bytes, ip_payload_protocols current_type);

                friend TPL_API bool operator==(ipv6_ext_headers_iterator const &lhs, ipv6_ext_headers_iterator const &rhs);

                friend TPL_API bool operator!=(ipv6_ext_headers_iterator const &lhs, ipv6_ext_headers_iterator const &rhs);

                ipv6_ext_headers_iterator &operator++();

                ip_v6_extension_header const &operator*() const;

            private:
                ip_v6_extension_header current_;
                byte_array left_bytes_;
            };
        }


        class TPL_API ip_v6_extension_headers_sequence {
        public:
            ip_v6_extension_headers_sequence();

            explicit ip_v6_extension_headers_sequence(byte_array const &bytes, ip_payload_protocols ipv6_next_header);

            iterators::ipv6_ext_headers_iterator begin() const;

            iterators::ipv6_ext_headers_iterator end() const;

            /// \brief function enumerates all ipv6 extension header and calls given callback on each header
            /// \param bytes ip v6 packet payload bytes,right after fixed header,
            /// containing extension headers (>=0)
            /// \param first_header -type of first extension header, taken from ipv6 header
            /// \param on_next_header callback to be called on each found header
            /// must have signature void (*)(ip_v6_extension_header const&)
            /// \return prefix of param array, containing all extension headers and no more byte
            template<class t_callback>
            static void enumerate_extension_headers(byte_array const &bytes,
                                                    ip_payload_protocols first_header,
                                                    t_callback const &on_next_header) {
                auto array_left = bytes;
                auto next_header = first_header;
                while (array_left.size() >= ip_v6_extension_header::fields::header_length &&
                       ip_v6_extension_header::is_extension_header_type(next_header)) {
                    auto next_header_length = ip_v6_extension_header::next_header_size(array_left, next_header);
                    auto extension_header = ip_v6_extension_header(array_left.prefix(next_header_length),
                                                                   next_header);
                    on_next_header(extension_header);
                    next_header = extension_header.next_header();
                    array_left = array_left.suffix(next_header_length);
                }
            }

        private:
            byte_array bytes_;
            ip_payload_protocols ipv6_next_header_{ip_payload_protocols::none};
        };
    }
}


#endif //PLAIN_PACKETS_EP_V6_EXTENSION_HEADERS_SEQUENCE_HPP
