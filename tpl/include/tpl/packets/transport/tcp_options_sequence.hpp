//
// Created by ukhegg on 24.10.2016.
//

#ifndef PLAIN_PACKETS_TCP_OPTIONS_SEQUENCE_HPP
#define PLAIN_PACKETS_TCP_OPTIONS_SEQUENCE_HPP

#include "tpl/packets/transport/tcp_option.hpp"

namespace tpl {
    namespace packets {
        class TPL_API tcp_options_iterator : public std::iterator<std::forward_iterator_tag, tcp_option> {
        public:
            tcp_options_iterator();

	        explicit tcp_options_iterator(byte_array const& bytes);;

            friend TPL_API bool operator==(tcp_options_iterator const &lhs, tcp_options_iterator const &rhs);

            friend TPL_API bool operator!=(tcp_options_iterator const &lhs, tcp_options_iterator const &rhs);

            tcp_options_iterator &operator++();

            tcp_option operator*() const;
        private:
			byte_array bytes_;
        };



        class TPL_API tcp_options_sequence {
        public:
            tcp_options_sequence();

	        explicit tcp_options_sequence(byte_array const &bytes);

            tcp_options_iterator begin() const;

            tcp_options_iterator end() const;
        private:
            byte_array bytes_;
        };
    }
}


#endif //PLAIN_PACKETS_TCP_OPTIONS_SEQUENCE_HPP
