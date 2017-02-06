#pragma once
#include "tpl/tpl.hpp"
#include <boost/asio/ip/address.hpp>

namespace tpl {
    namespace utils {
        class TPL_API ip_direction {
        public:
            using address_type = boost::asio::ip::address;

            ip_direction();

            ip_direction(address_type const &src, address_type const &dst);

            static ip_direction from_string(std::string const &src, std::string const &dst);

            ~ip_direction();

            address_type const &source() const;

            address_type const &destination() const;

            ip_direction reverce() const;

            bool is_reverce_to(ip_direction const &rhv) const;

	        friend TPL_API bool operator==(ip_direction const& lhs, ip_direction const& rhs);

	        friend TPL_API bool operator!=(ip_direction const& lhs, ip_direction const& rhs);

	        friend TPL_API bool operator<(ip_direction const& lhs, ip_direction const& rhs);

	        friend TPL_API bool operator<=(ip_direction const& lhs, ip_direction const& rhs);

	        friend TPL_API bool operator>(ip_direction const& lhs, ip_direction const& rhs);

	        friend TPL_API bool operator>=(ip_direction const& lhs, ip_direction const& rhs);

        private:
            address_type src_;
            address_type dst_;
        };
    }
}

