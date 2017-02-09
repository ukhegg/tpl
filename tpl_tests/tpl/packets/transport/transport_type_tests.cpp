//
// Created by ukheg on 03.12.2016.
//

#include <gtest/gtest.h>
#include <tpl/packets/transport/transport_type.hpp>

using namespace tpl;
using namespace packets;

namespace tpl_tests {
    TEST(transport_type_tests, io_tests) {
        auto to_string = [](transport_type tt) {
            std::ostringstream oss;
            oss << tt;
            return oss.str();
        };

        ASSERT_EQ("udp", to_string(transport_type::udp));
        ASSERT_EQ("tcp", to_string(transport_type::tcp));
        ASSERT_EQ("unknown transport type(666)", to_string(static_cast<transport_type>(666)));
    }

    TEST(transport_type_tests, wchar_io_tests) {
        auto to_string = [](transport_type tt) {
            std::wostringstream oss;
            oss << tt;
            return oss.str();
        };

        ASSERT_EQ(L"udp", to_string(transport_type::udp));
        ASSERT_EQ(L"tcp", to_string(transport_type::tcp));
        ASSERT_EQ(L"unknown transport type(666)", to_string(static_cast<transport_type>(666)));
    }
}

