
#include "test_helper.hpp"

#ifdef _GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion-null"
#endif

#include <boost/uuid/random_generator.hpp>

#ifdef _GCC
#pragma GCC diagnostic pop
#endif


#include <boost/uuid/uuid_io.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

using namespace tpl;
namespace tpl_tests
{
	test_helper_impl::test_helper_impl(){}

	test_helper_impl::~test_helper_impl()
	{
	}


	boost::filesystem::path test_helper_impl::get_temp_file() 
	{
		boost::uuids::basic_random_generator<boost::mt19937> gen;

		boost::filesystem::path test_folder = "test_folder";
		if (!exists(test_folder))
		{
			boost::filesystem::detail::create_directories(test_folder);
		}

		auto f = absolute(test_folder / to_wstring(gen()));
		this->temp_files_.push_back(f);
		return f;
	}
}
