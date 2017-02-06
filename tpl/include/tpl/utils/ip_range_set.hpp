#pragma once

#include "tpl/utils/ip_address_range.hpp"
#include <set>
#include <boost/icl/split_interval_map.hpp>

namespace tpl {
    namespace utils {
        class TPL_API ip_range_set {
        public:
            using ranges_map_type = boost::icl::split_interval_map<boost::asio::ip::address, std::set<ip_address_range>>;
            using const_iterator_type = std::set<ip_address_range>::const_iterator;

            ip_range_set();

            ip_range_set(std::initializer_list<ip_address_range> const &ranges);

            ~ip_range_set();

            inline const_iterator_type begin() const {
                return this->ranges_.begin();
            }

            inline const_iterator_type end() const {
                return this->ranges_.end();
            }

            std::set<ip_address_range> const &ranges() const {
                return this->ranges_;
            }

            void add(const ip_address_range &range);

            void add(const boost::asio::ip::address &addr);

            bool remove(const ip_address_range &range);

            bool contains(const boost::asio::ip::address &addr) const;

            std::set<ip_address_range> find_includings(boost::asio::ip::address const &addr) const;

            std::set<ip_address_range> find_intersections(ip_address_range const &range) const;

        private:
            std::set<ip_address_range> ranges_;
            ranges_map_type ranges_at_address_;
        };
    }
}

