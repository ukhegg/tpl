#include "tpl/formats/pcap/pcap_ostream.hpp"
#include "tpl/formats/pcap/detail/pcap_record_header.h"
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost::posix_time;

namespace tpl {
    namespace formats {
        pcap_ostream::pcap_ostream() {}

        pcap_ostream::pcap_ostream(std::ostream *output_stream)
                : state_(std::make_shared<state>(output_stream)) {}

        pcap_ostream::~pcap_ostream() {}

        void pcap_ostream::close() {
            if (this->state_->output_stream_ == nullptr) return;
            delete this->state_->output_stream_;
            this->state_->output_stream_ = nullptr;
        }

        void pcap_ostream::flush() {
            if (this->state_->output_stream_ != nullptr) this->state_->output_stream_->flush();
        }

        void pcap_ostream::write(const pcap_record &record) {
            this->write(record.bytes, record.time);
        }

        void pcap_ostream::write(byte_array const &bytes, ptime const &t) {
            detail::pcap_record_header record_header;
            record_header.packet_length = record_header.bytes_saved = static_cast<uint32_t>(bytes.size());

            const static ptime epoch(boost::gregorian::date(1970, boost::date_time::months_of_year::Jan, 1));
            auto time_since_epoch = t - epoch;
            record_header.timestamp_seconds = time_since_epoch.total_seconds();
            auto second_parts_left = time_since_epoch - seconds(1) * time_since_epoch.total_seconds();
            record_header.timestamp_milliseconds = static_cast<uint32_t>(second_parts_left.total_milliseconds());

            this->state_->output_stream_->write(reinterpret_cast<const char *>(&record_header),
                                                sizeof(detail::pcap_record_header));
            this->state_->bytes_written_ += sizeof(detail::pcap_record_header);
            this->state_->output_stream_->write(reinterpret_cast<const char *>(bytes.data()), bytes.size());
            this->state_->bytes_written_ += bytes.size();
        }

        pcap_ostream &pcap_ostream::operator<<(pcap_record const &r) {
            this->write(r);
            return *this;
        }

        pcap_ostream::state::state(std::ostream *output_stream) : output_stream_(output_stream) {}

        pcap_ostream::state::~state() {
            if (this->output_stream_ != nullptr) {
                this->output_stream_->flush();
                delete this->output_stream_;
            }
        }
    }
}

