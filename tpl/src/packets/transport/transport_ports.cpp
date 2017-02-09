#include <tpl/packets/transport/transport_ports.hpp>
#include <boost/optional/optional_io.hpp>

namespace tpl {
    namespace packets {
	    transport_port::transport_port()
	    {
	    }

	    transport_port::transport_port(transport_type p_type): type_(p_type)
	    {
	    }

	    transport_port::transport_port(uint16_t port, transport_type type): port_(port),
	                                                                        type_(type)
	    {
	    }

	    uint16_t transport_port::port() const
	    {
		    return this->port_;
	    }

	    transport_type transport_port::type() const
	    {
		    return *this->type_;
	    }

		bool operator==(transport_port const& lhs, transport_port const& rhs)
		{
			return lhs.port_ == rhs.port_
				&& lhs.type_ == rhs.type_;
		}

		bool operator!=(transport_port const& lhs, transport_port const& rhs)
		{
			return !(lhs == rhs);
		}

		bool operator<(transport_port const& lhs, transport_port const& rhs)
		{
			if (lhs.port_ < rhs.port_) return true;
			if (rhs.port_ < lhs.port_) return false;
			return lhs.type_ < rhs.type_;
		}

		bool operator<=(transport_port const& lhs, transport_port const& rhs)
		{
			return !(rhs < lhs);
		}

		bool operator>(transport_port const& lhs, transport_port const& rhs)
		{
			return rhs < lhs;
		}

		bool operator>=(transport_port const& lhs, transport_port const& rhs)
		{
			return !(lhs < rhs);
		}

        std::ostream &operator<<(std::ostream &os, const transport_port &port) {
            os << "port: " << port.port_ << " type: " << port.type_;
            return os;
        }

        udp_port::udp_port(): transport_port(transport_type::udp)
	    {
	    }

	    udp_port::udp_port(uint16_t port): transport_port(port, transport_type::udp)
	    {
	    }

	    tcp_port::tcp_port(): transport_port(transport_type::tcp)
	    {
	    }

	    tcp_port::tcp_port(uint16_t port): transport_port(port, transport_type::tcp)
	    {
	    }

	    transport_ports::transport_ports()
	    {
	    }

	    transport_ports::transport_ports(transport_type p_type): type_(p_type)
	    {
	    }

	    transport_ports::transport_ports(uint16_t source_port, uint16_t destination_port, transport_type p_type)
    	: src_port_(source_port), dst_port_(destination_port), type_(p_type)
	    {
	    }

	    transport_port transport_ports::src_port() const
	    {
			return transport_port(this->src_port_, *this->type_);
	    }

	    transport_port transport_ports::dst_port() const
	    {
			return transport_port(this->dst_port_, *this->type_);
	    }

		bool operator==(const transport_ports& lhs, const transport_ports& rhs)
		{
			return lhs.src_port_== rhs.src_port_
				&& lhs.dst_port_ == rhs.dst_port_
				&& lhs.type_ == rhs.type_;
		}

		bool operator!=(const transport_ports& lhs, const transport_ports& rhs)
		{
			return !(lhs == rhs);
		}

		bool operator<(const transport_ports& lhs, const transport_ports& rhs)
		{
			if (lhs.src_port_ < rhs.src_port_)
				return true;
			if (rhs.src_port_ < lhs.src_port_)
				return false;
			if (lhs.dst_port_ < rhs.dst_port_)
				return true;
			if (rhs.dst_port_ < lhs.dst_port_)
				return false;
			return lhs.type_ < rhs.type_;
		}

		bool operator<=(const transport_ports& lhs, const transport_ports& rhs)
		{
			return !(rhs < lhs);
		}

		bool operator>(const transport_ports& lhs, const transport_ports& rhs)
		{
			return rhs < lhs;
		}

		bool operator>=(const transport_ports& lhs, const transport_ports& rhs)
		{
			return !(lhs < rhs);
		}

		std::ostream &operator<<(std::ostream &os, const transport_ports &ports) {
			os << "src_port: " << ports.src_port_ << " dst_port: " << ports.dst_port_ << " type: " << ports.type_;
			return os;
		}


		tcp_ports::tcp_ports(): transport_ports(transport_type::tcp)
	    {
	    }

	    tcp_ports::tcp_ports(uint16_t src, uint16_t dst): transport_ports(src, dst, transport_type::tcp)
	    {
	    }

	    port_interval::port_interval()
	    {
	    }

	    port_interval::port_interval(transport_type t): type_(t)
	    {
	    }

	    port_interval::port_interval(uint16_t p_first, uint16_t p_last, transport_type t)
			: type_(t), first_(p_first), last_(p_last)
	    {
	    }

	    transport_type port_interval::type() const
	    {
		    return *type_;
	    }

	    uint16_t port_interval::first() const
	    {
		    return first_;
	    }

	    uint16_t port_interval::last() const
	    {
		    return last_;
	    }

		bool operator==(const port_interval& lhs, const port_interval& rhs)
		{
			return lhs.type_ == rhs.type_
				&& lhs.first_ == rhs.first_
				&& lhs.last_ == rhs.last_;
		}

		bool operator!=(const port_interval& lhs, const port_interval& rhs)
		{
			return !(lhs == rhs);
		}

		bool operator<(const port_interval& lhs, const port_interval& rhs)
		{
			if (lhs.type_ < rhs.type_)
				return true;
			if (rhs.type_ < lhs.type_)
				return false;
			if (lhs.first_ < rhs.first_)
				return true;
			if (rhs.first_ < lhs.first_)
				return false;
			return lhs.last_ < rhs.last_;
		}

		bool operator<=(const port_interval& lhs, const port_interval& rhs)
		{
			return !(rhs < lhs);
		}

		bool operator>(const port_interval& lhs, const port_interval& rhs)
		{
			return rhs < lhs;
		}

		bool operator>=(const port_interval& lhs, const port_interval& rhs)
		{
			return !(lhs < rhs);
		}

        std::ostream& operator<<(std::ostream &os, const port_interval &interval) {
            os << "type: " << interval.type_ << " first: " << interval.first_ << " last: " << interval.last_;
            return os;
        }

        udp_ports_interval::udp_ports_interval(): port_interval(transport_type::udp)
	    {
	    }

	    udp_ports_interval::udp_ports_interval(uint16_t from, uint16_t to): port_interval(from, to, transport_type::udp)
	    {
	    }

	    tcp_ports_interval::tcp_ports_interval(): port_interval(transport_type::tcp)
	    {
	    }

	    tcp_ports_interval::tcp_ports_interval(uint16_t from, uint16_t to): port_interval(from, to, transport_type::tcp)
	    {
	    }

	    port_interval udp_single_port_interval(uint16_t p_first)
	    {
		    return udp_ports_interval(p_first, p_first);
	    }

	    port_interval tcp_single_port_interval(uint16_t p_first)
	    {
		    return tcp_ports_interval( p_first, p_first);
	    }
    }
}

