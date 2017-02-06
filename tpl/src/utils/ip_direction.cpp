#include <tpl/utils/ip_direction.hpp>


namespace tpl {
    namespace utils {
        ip_direction::ip_direction() {}

        ip_direction::ip_direction(address_type const &src, address_type const &dst)
                : src_(src), dst_(dst) {}

        ip_direction ip_direction::from_string(std::string const &src, std::string const &dst) {
            return ip_direction(address_type::from_string(src),
                                address_type::from_string(dst));
        }

        ip_direction::~ip_direction() {}

        ip_direction::address_type const &ip_direction::source() const {
            return this->src_;
        }

        ip_direction::address_type const &ip_direction::destination() const {
            return this->dst_;
        }

        ip_direction ip_direction::reverce() const {
            return ip_direction(this->dst_, this->src_);
        }

        bool ip_direction::is_reverce_to(ip_direction const &rhv) const {
            return this->src_ == rhv.dst_ && this->dst_ == rhv.src_;
        }

	    bool operator==(ip_direction const& lhs, ip_direction const& rhs)
	    {
		    return lhs.src_ == rhs.src_
			    && lhs.dst_ == rhs.dst_;
	    }

	    bool operator!=(ip_direction const& lhs, ip_direction const& rhs)
	    {
		    return !(lhs == rhs);
	    }

	    bool operator<(ip_direction const& lhs, ip_direction const& rhs)
	    {
		    if (lhs.src_ < rhs.src_) return true;
		    if (rhs.src_ < lhs.src_) return false;
		    return lhs.dst_ < rhs.dst_;
	    }

	    bool operator<=(ip_direction const& lhs, ip_direction const& rhs)
	    {
		    return !(rhs < lhs);
	    }

	    bool operator>(ip_direction const& lhs, ip_direction const& rhs)
	    {
		    return rhs < lhs;
	    }

	    bool operator>=(ip_direction const& lhs, ip_direction const& rhs)
	    {
		    return !(lhs < rhs);
	    }
    }
}

