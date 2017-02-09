#include <gtest/gtest.h>
#include "test_helper.hpp"
#include <tpl/formats/pcap/pcap_file.hpp>
#include <list>
using namespace tpl;
using namespace tpl::formats;

namespace tpl_tests
{
	TEST(pcap_file_tests, can_open_reader)
	{
		auto reader = pcap_file(test_helper.get_test_file("eth_ip_https.pcap")).open_reader();
	}

	TEST(pcap_file_tests, can_read_record)
	{
		auto reader = pcap_file(test_helper.get_test_file("eth_ip_https.pcap")).open_reader();
		pcap_record r;
		ASSERT_TRUE(reader.read_next(r));

		ASSERT_EQ(size_t(90), r.bytes.size());
		ASSERT_EQ(uint8_t(0x33), r.bytes[0]);
	}

	TEST(pcap_file_tests, can_read_file)
	{
		auto reader = pcap_file(test_helper.get_test_file("eth_ip_https.pcap")).open_reader();
		std::list<pcap_record> records;
		pcap_record r;
		while(reader.read_next(r)) records.push_back(r);

		ASSERT_EQ(size_t(81), records.size());
	}

	TEST(pcap_file_tests, distributes_read_bytes_count_among_reader_copies)
	{
		auto reader = pcap_file(test_helper.get_test_file("eth_ip_https.pcap")).open_reader();

		auto reader2 = reader;
		pcap_record r;
		reader.read_next(r);

		ASSERT_TRUE(reader.bytes_read() > 0);
		ASSERT_EQ(reader.bytes_read(), reader2.bytes_read());
	}

	TEST(pcap_file_tests, doesnt_close_ostream_until_pcap_ostream_instance_exists)
	{
		pcap_ostream pcap_ostrm;

		{
			auto writer = pcap_file(test_helper.get_temp_file()).open_writer();
			pcap_ostrm = writer;
		}

		ASSERT_TRUE(pcap_ostrm.is_open());
	}

	TEST(pcap_file_tests, throws_if_cant_open_file_for_reading)
	{
		auto not_existing_file = "Z:\\some unexisting file.pcap";
		ASSERT_THROW(pcap_file(not_existing_file).open_reader(), std::invalid_argument);
	}

	TEST(pcap_file_tests, throws_if_cant_open_file_for_writing)
	{
		auto not_existing_file = "Z:\\some unexisting file.pcap";
		ASSERT_THROW(pcap_file(not_existing_file).open_writer(), std::invalid_argument);
	}

	TEST(pcap_file_tests, can_iterate_over_packets_using_for_loop)
	{
		auto f = pcap_file(test_helper.get_test_file("eth_ip_https.pcap"));
		std::vector<pcap_record> records;
		for(auto&& r : f.records()) records.push_back(r);
		ASSERT_EQ(81, records.size());
	}

	TEST(pcap_file_tests, can_iterate_over_packets_using_classic_iterators)
	{
		std::vector<pcap_record> records;
		auto pcap_records = pcap_file(test_helper.get_test_file("eth_ip_https.pcap")).records();
		for(auto it = pcap_records.begin() ;it != pcap_records.end() ;++it)
		{
			records.push_back(*it);
		}
		ASSERT_EQ(81, records.size());
	}

	TEST(pcap_file_tests, can_iterate_over_empty_file)
	{
		auto empty_file = test_helper.get_temp_file().replace_extension(".pcap");
		pcap_file(empty_file).open_writer();

		pcap_file bf(empty_file);

		std::vector<pcap_record> records;
		for(auto const& r : bf.records()) records.push_back(r);
		ASSERT_EQ(0, records.size());
	}
}

