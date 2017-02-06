#pragma once

#include "tpl/tpl.hpp"
#include <boost/asio/ip/address.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/nvp.hpp>
#include <memory.h>

namespace tpl {
    namespace utils {
        namespace detail {
            class ip_address_range_impl;
        }

        //////////////////////////////////////////////////////////////////////////
        // ip_address_range
        // represents a continuous ip address range
        //////////////////////////////////////////////////////////////////////////
        class TPL_API ip_address_range {
        public:
            static ip_address_range any_v4_address();
            static ip_address_range any_v6_address();

            ip_address_range();

            explicit ip_address_range(const boost::asio::ip::address &addr);

            explicit ip_address_range(const std::string &addr);

            ip_address_range(const boost::asio::ip::address &first, const boost::asio::ip::address &last);

            ip_address_range(const std::string &first, const std::string &last);

            ~ip_address_range();

            boost::asio::ip::address first() const;

            boost::asio::ip::address last() const;

            bool is_v4() const;

            bool is_v6() const;

            bool is_single_address() const;

            bool contains(const boost::asio::ip::address &addr) const;

            bool contains(const std::string &addr) const;

            bool contains(const ip_address_range &range) const;

            bool operator<(const ip_address_range &range) const;

            bool operator==(const ip_address_range &range) const;

            bool operator==(std::nullptr_t) const;

            std::string to_string() const;

            std::wstring to_wstring() const;

            operator boost::asio::ip::address() const;

        private:
            explicit ip_address_range(const std::shared_ptr<detail::ip_address_range_impl> &impl);

            std::shared_ptr<detail::ip_address_range_impl> pimpl_;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                boost::serialization::split_member(ar, *this, version);
            }

            template<class Archive>
            void save(Archive &ar, const unsigned int version) const {
                auto first = this->first().to_string();
                auto last = this->last().to_string();
                ar & BOOST_SERIALIZATION_NVP(first);
                ar & BOOST_SERIALIZATION_NVP(last);
            }

            template<class Archive>
            void load(Archive &ar, const unsigned int version) {
                std::string first, last;
                ar & boost::serialization::make_nvp("first", first);
                ar & boost::serialization::make_nvp("last", last);

                *this = ip_address_range(first, last);
            }
        };
    }
}

