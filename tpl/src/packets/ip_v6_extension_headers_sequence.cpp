//
// Created by ukheg on 02.10.2016.
//

#include "tpl/packets/ip_v6_extension_headers_sequence.hpp"


namespace tpl {
    namespace packets {
        namespace iterators {
            ipv6_ext_headers_iterator::ipv6_ext_headers_iterator() {

            }

            ipv6_ext_headers_iterator::ipv6_ext_headers_iterator(byte_array const &bytes,
                                                                 ip_payload_protocols current_type) {
                auto first_length = ip_v6_extension_header::next_header_size(bytes, current_type);
                this->current_ = ip_v6_extension_header(bytes.prefix(first_length), current_type);
                this->left_bytes_ = bytes.suffix(first_length);
            }

            bool operator==(ipv6_ext_headers_iterator const &lhs,
                            ipv6_ext_headers_iterator const &rhs) {
                auto arrays_are_same = [](byte_array const &lhs, byte_array const &rhs) {
                    return lhs.data() == rhs.data() && lhs.size() == rhs.size();
                };

                return arrays_are_same(lhs.left_bytes_, rhs.left_bytes_) &&
                       arrays_are_same(lhs.current_.bytes(), rhs.current_.bytes());
            }

            bool operator!=(ipv6_ext_headers_iterator const &lhs,
                            ipv6_ext_headers_iterator const &rhs) {
                return !(lhs == rhs);
            }

            ipv6_ext_headers_iterator &ipv6_ext_headers_iterator::operator++() {
                auto next_header = this->current_.next_header();
                if (!ip_v6_extension_header::is_extension_header_type(next_header)) {
                    this->current_ = ip_v6_extension_header();
                    this->left_bytes_.reset();
                    return *this;
                }
                auto next_header_bytes = this->left_bytes_.prefix(this->current_.total_length());
                this->current_ = ip_v6_extension_header(next_header_bytes, next_header);
                this->left_bytes_ = this->left_bytes_.suffix(next_header_bytes.size());
                return *this;
            }

            ip_v6_extension_header const &ipv6_ext_headers_iterator::operator*() const {
                return this->current_;
            }
        }

        ip_v6_extension_headers_sequence::ip_v6_extension_headers_sequence() {

        }

        ip_v6_extension_headers_sequence::ip_v6_extension_headers_sequence(const byte_array &bytes,
                                                                           ip_payload_protocols ipv6_next_header)
                : bytes_(bytes) {
            if (ip_v6_extension_header::is_extension_header_type(ipv6_next_header)) {
                this->ipv6_next_header_ = ipv6_next_header;
            }
        }

        iterators::ipv6_ext_headers_iterator ip_v6_extension_headers_sequence::begin() const {
            return ipv6_next_header_ != ip_payload_protocols::none
                   ? iterators::ipv6_ext_headers_iterator(this->bytes_, this->ipv6_next_header_)
                   : iterators::ipv6_ext_headers_iterator();
        }

        iterators::ipv6_ext_headers_iterator ip_v6_extension_headers_sequence::end() const {
            return iterators::ipv6_ext_headers_iterator();
        }
    }
}