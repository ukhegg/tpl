#pragma once

#include "tpl/tpl.hpp"
#include "tpl/formats/pcap/pcap_record.hpp"

namespace tpl {
    namespace formats {
        class TPL_API pcap_ostream {
        public:
            pcap_ostream();

            explicit pcap_ostream(std::ostream *output_stream);

            ~pcap_ostream();

            void close();

            void flush();

            pcap_ostream &operator<<(pcap_record const &r);

            void write(const pcap_record &record);

            void write(byte_array const &bytes, boost::posix_time::ptime const &t);

            bool operator==(std::nullptr_t) const {
                return this->state_ == nullptr;
            }

            bool operator!=(std::nullptr_t) const {
                return this->state_ != nullptr;
            }

            bool is_open() const {
                return this->state_->output_stream_ != nullptr;
            }

            uint64_t bytes_written() const {
                return this->state_->bytes_written_;
            }

        private:
            struct state {
                explicit state(std::ostream *output_stream);

                ~state();

                std::ostream *output_stream_{nullptr};
                uint64_t bytes_written_{0};
            };

            std::shared_ptr<state> state_;
        };
    }
}

