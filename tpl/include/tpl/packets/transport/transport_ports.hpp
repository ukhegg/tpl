#pragma once
#include <cstddef>
#include <stdint.h>
#include "tpl/tpl.hpp"
#include "transport_type.hpp"
#include <boost/optional.hpp>
#include <iostream>

namespace tpl {
    namespace packets {
        class TPL_API transport_port {
        public:
	        transport_port();

	        explicit transport_port(transport_type p_type);

	        transport_port(uint16_t port, transport_type type);

	        uint16_t port() const;

	        transport_type type() const;

	        friend TPL_API bool operator==(transport_port const& lhs, transport_port const& rhs);
	        friend TPL_API bool operator!=(transport_port const& lhs, transport_port const& rhs);
	        friend TPL_API bool operator<(transport_port const& lhs, transport_port const& rhs);
	        friend TPL_API bool operator<=(transport_port const& lhs, transport_port const& rhs);
	        friend TPL_API bool operator>(transport_port const& lhs, transport_port const& rhs);
	        friend TPL_API bool operator>=(transport_port const& lhs, transport_port const& rhs);
			friend TPL_API std::ostream &operator<<(std::ostream &os, const transport_port &port);
		private:
			uint16_t port_{ 0 };
			boost::optional<transport_type> type_;
        };

		class TPL_API udp_port : public transport_port
		{
		public:
			udp_port();

			explicit udp_port(uint16_t port);
		};

		class TPL_API tcp_port : public transport_port
		{
		public:
			tcp_port();

			explicit tcp_port(uint16_t port);
		};

        class TPL_API transport_ports {
        public:
	        transport_ports();

	        explicit transport_ports(transport_type p_type);

	        transport_ports(uint16_t source_port, uint16_t destination_port, transport_type p_type);

	        transport_port src_port() const;
	        transport_port dst_port() const;
			transport_type type() const { return  *this->type_; }

	        friend TPL_API bool operator==(const transport_ports& lhs, const transport_ports& rhs);
	        friend TPL_API bool operator!=(const transport_ports& lhs, const transport_ports& rhs);

	        friend TPL_API bool operator<(const transport_ports& lhs, const transport_ports& rhs);
	        friend TPL_API bool operator<=(const transport_ports& lhs, const transport_ports& rhs);
	        friend TPL_API bool operator>(const transport_ports& lhs, const transport_ports& rhs);
	        friend TPL_API bool operator>=(const transport_ports& lhs, const transport_ports& rhs);
            friend TPL_API std::ostream &operator<<(std::ostream &os, const transport_ports &ports);
        private:
			uint16_t src_port_{ 0 };
			uint16_t dst_port_{ 0 };
			boost::optional<transport_type> type_;
        };

		class TPL_API udp_ports : public transport_ports
		{
		public:
			udp_ports() : transport_ports(transport_type::udp){}

			udp_ports(uint16_t src, uint16_t dst) : transport_ports(src, dst, transport_type::udp){}
		};

		class TPL_API tcp_ports : public transport_ports
		{
		public:
			tcp_ports();

			tcp_ports(uint16_t src, uint16_t dst);
		};

        class TPL_API port_interval {
        public:
	        port_interval();

	        explicit port_interval(transport_type t);

	        port_interval(uint16_t p_first, uint16_t p_last, transport_type t);

	        transport_type type() const;

	        uint16_t first() const;

	        uint16_t last() const;

	        friend TPL_API bool operator==(const port_interval& lhs, const port_interval& rhs);
	        friend TPL_API bool operator!=(const port_interval& lhs, const port_interval& rhs);

	        friend TPL_API bool operator<(const port_interval& lhs, const port_interval& rhs);
	        friend TPL_API bool operator<=(const port_interval& lhs, const port_interval& rhs);
	        friend TPL_API bool operator>(const port_interval& lhs, const port_interval& rhs);
	        friend TPL_API bool operator>=(const port_interval& lhs, const port_interval& rhs);
            friend TPL_API std::ostream &operator<<(std::ostream &os, const port_interval &interval);
        private:
			boost::optional<transport_type> type_;
			uint16_t first_{ 0 };
			uint16_t last_{ 0 };
        };

		class TPL_API  udp_ports_interval : public port_interval
		{
		public:
			udp_ports_interval();

			udp_ports_interval(uint16_t from, uint16_t to);
		};

		class TPL_API  tcp_ports_interval : public port_interval
		{
		public:
			tcp_ports_interval();

			tcp_ports_interval(uint16_t from, uint16_t to);
		};

		TPL_API port_interval udp_single_port_interval(uint16_t p_first);

		TPL_API port_interval tcp_single_port_interval(uint16_t p_first);
    }
}

