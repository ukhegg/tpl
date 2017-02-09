#include <tpl/formats/pcap/pcap_records_container.hpp>


namespace tpl {
    namespace formats {
        pcap_stream_iterator::pcap_stream_iterator() {}

        pcap_stream_iterator::pcap_stream_iterator(pcap_istream const &istream) : istream_(istream) {
            pcap_record r;
            if (this->istream_.read_next(r)) {
                this->current_ = r;
            } else {
                this->istream_ = pcap_istream();
            }
        }

        pcap_stream_iterator &pcap_stream_iterator::operator++() {
            if (this->istream_ == nullptr) return *this;

            pcap_record r;
            if (!this->istream_.read_next(r)) {
                this->current_ = boost::none;
                this->istream_ = pcap_istream();
            } else {
                this->current_ = r;
            }
            return *this;
        }

        pcap_record const &pcap_stream_iterator::operator*() const {
            return *this->current_;
        }


        bool operator==(pcap_stream_iterator const &lhs, pcap_stream_iterator const &rhs) {
            return lhs.istream_ == rhs.istream_;
        }

        bool operator!=(pcap_stream_iterator const &lhs, pcap_stream_iterator const &rhs) {
            return !(lhs == rhs);
        }

        pcap_records_container::pcap_records_container() {}

        pcap_records_container::pcap_records_container(pcap_istream const &is)
                : istream_(is) {}

        pcap_records_container::~pcap_records_container() {}

        pcap_stream_iterator pcap_records_container::begin() const {
            return pcap_stream_iterator(this->istream_);
        }

        pcap_stream_iterator pcap_records_container::end() const {
            return pcap_stream_iterator();
        }
    }
}

