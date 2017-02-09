//
// Created by ukhegg on 24.10.2016.
//

#include "tpl/packets/transport/tcp_options_sequence.hpp"

namespace tpl{
    namespace packets{
        tcp_options_iterator::tcp_options_iterator() {}

        tcp_options_iterator::tcp_options_iterator(byte_array const &bytes) : bytes_(bytes){
            if(this->bytes_.is_empty()) this->bytes_ = byte_array();
        }

        bool operator==(tcp_options_iterator const &lhs, tcp_options_iterator const &rhs) {
            return lhs.bytes_.data() == rhs.bytes_.data() &&
                   lhs.bytes_.size() == rhs.bytes_.size();
        }

        bool operator!=(tcp_options_iterator const &lhs, tcp_options_iterator const &rhs) {
            return lhs.bytes_.data() != rhs.bytes_.data() ||
                   lhs.bytes_.size() != rhs.bytes_.size();
        }

        tcp_options_iterator &tcp_options_iterator::operator++() {
            if(this->bytes_.is_empty()) throw std::runtime_error("iterating over end of options sequence");
            auto current_option_length = tcp_option::option_size(this->bytes_);
            this->bytes_ = this->bytes_.suffix(current_option_length);
            if(bytes_.is_empty()){
                bytes_ = byte_array();
            }
			return *this;
        }

        tcp_option tcp_options_iterator::operator*() const {
            auto current_option_length = tcp_option::option_size(this->bytes_);
            return tcp_option(this->bytes_.prefix(current_option_length));
        }

        tcp_options_sequence::tcp_options_sequence() {}

        tcp_options_sequence::tcp_options_sequence(byte_array const &bytes) : bytes_(bytes){}

        tcp_options_iterator tcp_options_sequence::begin() const {
            return tcp_options_iterator(this->bytes_);
        }

        tcp_options_iterator tcp_options_sequence::end() const {
            return tcp_options_iterator();
        }
    }
}