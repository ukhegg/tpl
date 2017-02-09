#pragma once

#include "tpl/tpl.hpp"
#include "tpl/formats/pcap/pcap_record.hpp"
#include "tpl/formats/pcap/pcap_link_layer.h"
#include <boost/smart_ptr/detail/shared_count.hpp>
#include <utility>

namespace tpl {
    namespace formats {
        class TPL_API pcap_istream {
        public:
            pcap_istream();

            pcap_istream(std::istream *input_stream, pcap_link_layer link_layer);

            ~pcap_istream();

            pcap_link_layer link_layer() const {
                return this->state_->link_layer_;
            }

            void close();

            bool read_next(pcap_record &record);

            pcap_istream &operator>>(pcap_record &r);

            bool operator==(std::nullptr_t) const {
                return this->state_ == nullptr;
            }

            bool operator!=(std::nullptr_t) const {
                return this->state_ != nullptr;
            }


            friend bool operator==(pcap_istream const &lhs, pcap_istream const &rhs) {
                return lhs.state_ == rhs.state_;
            }

            friend bool operator!=(pcap_istream const &lhs, pcap_istream const &rhs) {
                return !(lhs == rhs);
            }

            uint64_t bytes_read() const {
                return this->state_->bytes_read_;
            }

            bool eof() const {
                return this->state_->input_stream->eof();
            }

            uint64_t records_read() const {
                return this->state_->record_read_;
            }

        private:
            struct state {
                state(std::istream *input_stream, pcap_link_layer link_layer);

                ~state();

                uint64_t bytes_read_{0};
                uint64_t record_read_{0};
                std::istream *input_stream;
                pcap_link_layer link_layer_;
            };

            std::shared_ptr<state> state_;
        };
    }
}

