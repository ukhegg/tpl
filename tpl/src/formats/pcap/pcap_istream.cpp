#include "tpl/formats/pcap/pcap_istream.hpp"
#include "tpl/formats/pcap/detail/pcap_record_header.h"
#include <boost/date_time/posix_time/posix_time.hpp>

namespace tpl {
    namespace formats {
        pcap_istream::pcap_istream() {}

        pcap_istream::pcap_istream(std::istream *input_stream, pcap_link_layer link_layer)
                : state_(std::make_shared<state>(input_stream, link_layer)) {
            if (input_stream == nullptr) throw std::invalid_argument("input stream is null");
        }

        pcap_istream::~pcap_istream() {}

        void pcap_istream::close() {
            if (this->state_ == nullptr) return;
            delete this->state_->input_stream;
            this->state_->input_stream = nullptr;
        }

        bool pcap_istream::read_next(pcap_record &record) {
            if (this->state_ == nullptr) throw std::invalid_argument("pcap-stream_reader not initialized");

            detail::pcap_record_header recordHeader;
            recordHeader << *this->state_->input_stream;
            if (this->eof()) return false;

            const static boost::posix_time::ptime epoch(
                    boost::gregorian::date(1970, boost::date_time::months_of_year::Jan, 1));
            record.time = epoch + boost::posix_time::seconds(recordHeader.timestamp_seconds)
                          + boost::posix_time::microseconds(recordHeader.timestamp_milliseconds);

            record.bytes = byte_array(recordHeader.bytes_saved);
            this->state_->input_stream->read(reinterpret_cast<char *>(record.bytes.data()), recordHeader.bytes_saved);

            this->state_->bytes_read_ += sizeof(detail::pcap_record_header) + recordHeader.bytes_saved;
            this->state_->record_read_++;
            return true;
        }

        pcap_istream &pcap_istream::operator>>(pcap_record &r) {
            this->read_next(r);
            return *this;
        }

        pcap_istream::state::state(std::istream *input_stream, pcap_link_layer link_layer) : input_stream(input_stream),
                                                                                             link_layer_(link_layer) {}

        pcap_istream::state::~state() {
            if (this->input_stream == nullptr) return;
            delete this->input_stream;
        }
    }
}

