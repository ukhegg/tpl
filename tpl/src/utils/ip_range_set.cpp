#include "tpl/utils/ip_range_set.hpp"
#include <tuple>
#include <boost/icl/set.hpp>


namespace tpl {
    namespace utils {
        ip_range_set::ip_range_set() {}

        ip_range_set::ip_range_set(std::initializer_list<ip_address_range> const &ranges) {
            for (auto const &r : ranges) {
                this->add(r);
            }
        }

        ip_range_set::~ip_range_set() {}

        void ip_range_set::add(ip_address_range const &range) {
            if (!this->ranges_.insert(range).second) return;

            auto target_range = boost::icl::interval<boost::asio::ip::address>::closed(range.first(), range.last());
            this->ranges_at_address_ += std::make_pair(target_range, std::set<ip_address_range>{range});
        }

        void ip_range_set::add(boost::asio::ip::address const &address) {
            this->add(ip_address_range(address));
        }

        bool ip_range_set::remove(const ip_address_range &range) {
            if (this->ranges_.erase(range) == 0) return false;
            auto target_range = boost::icl::interval<boost::asio::ip::address>::closed(range.first(), range.last());
            this->ranges_at_address_.erase(std::make_pair(target_range, std::set<ip_address_range>{range}));
            return true;
        }

        bool ip_range_set::contains(boost::asio::ip::address const &address) const {
            auto iter = this->ranges_at_address_.find(address);
            return iter != this->ranges_at_address_.end();
        }

        std::set<ip_address_range> ip_range_set::find_includings(boost::asio::ip::address const &address) const {
            std::set<ip_address_range> result;
            auto iter = this->ranges_at_address_.find(address);
            if (iter != this->ranges_at_address_.end()) {
                for (const auto &r : (*iter).second) result.insert(r);
            }
            return move(result);
        }

        std::set<ip_address_range> ip_range_set::find_intersections(ip_address_range const &range) const {
            std::set<ip_address_range> result;
            auto iter = this->ranges_at_address_.find(range.first());

            auto argument_range = boost::icl::interval<boost::asio::ip::address>::closed(range.first(), range.last());

            auto intersection_iter = this->ranges_at_address_.lower_bound(argument_range);
            while (intersection_iter != this->ranges_at_address_.end()) {
                auto const &intersetion_range = (*intersection_iter).first;
                if (!boost::icl::intersects(intersetion_range, argument_range)) break;

                for (auto const &r : (*intersection_iter).second) {
                    result.insert(r);
                }
                ++intersection_iter;
            }


            return move(result);
        }
    }
}

