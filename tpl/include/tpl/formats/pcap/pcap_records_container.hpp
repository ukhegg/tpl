#pragma once

#include "tpl/formats/pcap/pcap_istream.hpp"
#include <iterator>

namespace tpl
{
	namespace formats
	{
		class TPL_API pcap_stream_iterator : public std::iterator<std::input_iterator_tag,
		                                                          pcap_record>
		{
		public:
			pcap_stream_iterator();

			explicit pcap_stream_iterator(pcap_istream const& istream);

			friend TPL_API bool operator==(pcap_stream_iterator const& lhs, pcap_stream_iterator const& rhs);

			friend TPL_API bool operator!=(pcap_stream_iterator const& lhs, pcap_stream_iterator const& rhs);

			pcap_stream_iterator& operator++();

			pcap_record const& operator*() const;

		private:
			pcap_istream istream_;
			boost::optional<pcap_record> current_;
		};

		class TPL_API pcap_records_container
		{
		public:
			pcap_records_container();

			explicit pcap_records_container(pcap_istream const& is);

			~pcap_records_container();

			pcap_stream_iterator begin() const;

			pcap_stream_iterator end() const;

		private:
			pcap_istream istream_;
		};
	}
}
