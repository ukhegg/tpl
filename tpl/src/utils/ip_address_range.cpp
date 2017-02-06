#include "tpl/utils/ip_address_range.hpp"
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/type_traits/is_same.hpp>

using namespace boost::asio::ip;

namespace tpl {
    namespace utils {
        namespace detail {
            class ip_address_range_impl {
            public:
                virtual ~ip_address_range_impl() {}

                virtual bool is_v4() const = 0;

                virtual bool is_v6() const = 0;

                virtual address first() const = 0;

                virtual address last() const = 0;

                virtual bool contains_address(const address &addr) const = 0;

                virtual bool contains_range(const ip_address_range_impl *rangeImpl) const = 0;
            };

            template<class t_target>
            struct to_target {
                static t_target convert(const address &addr) {
                    static_assert(std::is_same<address_v4, t_target>::value || std::is_same<address_v6, t_target>::value,
                                  "invalid template parameter");
                }
            };

            template<>
            struct to_target<address_v4> {
                static address_v4 convert(const address &addr) {
                    return addr.to_v4();
                }
            };

            template<>
            struct to_target<address_v6> {
                static address_v6 convert(const address &addr) {
                    return addr.to_v6();
                }
            };

            template<class t_address>
            class ip_version_address_range_impl : public ip_address_range_impl {
            public:
                ip_version_address_range_impl(const t_address &first, const t_address &last)
                        : first_(std::min<t_address>(first, last)), last_(std::max<t_address>(first, last)) {}

                virtual ~ip_version_address_range_impl() {}

                bool is_v4() const override {
                    return boost::is_same<t_address, address_v4>::value;
                }

                bool is_v6() const override {
                    return boost::is_same<t_address, address_v6>::value;
                }

                address first() const override {
                    return this->first_;
                }

                address last() const override {
                    return this->last_;
                }

                bool contains_address(const address &addr) const override {
                    if (addr.is_v4() != this->is_v4()) return false;
                    t_address tAddr = to_target<t_address>::convert(addr);
                    return tAddr >= this->first_ && tAddr <= this->last_;
                }

                bool contains_range(const ip_address_range_impl *rangeImpl) const override {
                    if (this->is_v4() != rangeImpl->is_v4()) return false;
                    return this->first_ <= rangeImpl->first() &&
                           this->last_ >= rangeImpl->last();
                }

            private:
                t_address first_;
                t_address last_;
            };

            typedef ip_version_address_range_impl<address_v4> ip_v4_address_range_impl;
            typedef ip_version_address_range_impl<address_v6> ip_v6_address_range_impl;

            template<class t_address>
            class single_address_range_impl : public ip_address_range_impl {
            public:
                explicit single_address_range_impl(const t_address &addr) : address_(addr) {}

                virtual ~single_address_range_impl() {}

                bool is_v4() const override {
                    return boost::is_same<t_address, address_v4>::value;
                }

                bool is_v6() const override {
                    return boost::is_same<t_address, address_v6>::value;
                }

                address first() const override {
                    return this->address_;
                }

                address last() const override {
                    return this->address_;
                }

                bool contains_address(const address &addr) const override {
                    return this->address_ == addr;
                }

                bool contains_range(const ip_address_range_impl *rangeImpl) const override {
                    return rangeImpl->first() == address_ &&
                           rangeImpl->last() == address_;
                }

            private:
                t_address address_;
            };

            typedef single_address_range_impl<address_v4> single_ipv4_impl;
            typedef single_address_range_impl<address_v6> single_ipv6_impl;

            template<class t_address>
            class any_address_range_impl : public ip_address_range_impl {
            public:
                ~any_address_range_impl() override {}

                bool is_v4() const override {
                    return boost::is_same<t_address, address_v4>::value;
                }

                bool is_v6() const override {
                    return boost::is_same<t_address, address_v4>::value;
                }

                address first() const override {
                    return t_address();
                }

                address last() const override {
                    return t_address();
                }

                bool contains_address(const address &addr) const override {
                    return (addr.is_v4() && this->is_v4()) || (addr.is_v6() && this->is_v6());
                }

                bool contains_range(const ip_address_range_impl *rangeImpl) const override {
                    return (rangeImpl->is_v4() && this->is_v4()) || (rangeImpl->is_v6() && this->is_v6());
                }
            };
        }


	    ip_address_range ip_address_range::any_v4_address()
	    {
			static ip_address_range any_v4_addr(std::make_shared<detail::any_address_range_impl<address_v4>>());
			return  any_v4_addr;
	    }

	    ip_address_range ip_address_range::any_v6_address()
	    {
			static ip_address_range any_v6_addr(std::make_shared<detail::any_address_range_impl<address_v6>>());
			return  any_v6_addr;
	    }


	    ip_address_range::ip_address_range() {}

        ip_address_range::ip_address_range(const address &first, const address &last) {
            if (first.is_v4() && last.is_v4()) {
                if (first == last) {
                    this->pimpl_ = std::make_shared<detail::single_ipv4_impl>(first.to_v4());
                } else {
                    this->pimpl_ = std::make_shared<detail::ip_v4_address_range_impl>(first.to_v4(), last.to_v4());
                }
            } else if (first.is_v6() && last.is_v6()) {
                if (first == last) {
                    this->pimpl_ = std::make_shared<detail::single_ipv6_impl>(first.to_v6());
                } else {
                    this->pimpl_ = std::make_shared<detail::ip_v6_address_range_impl>(first.to_v6(), last.to_v6());
                }
            } else throw std::invalid_argument("first and last must be both v4 or v6");
        }

        ip_address_range::ip_address_range(const std::string &first, const std::string &last)
                : ip_address_range(address::from_string(first), address::from_string(last)) {}

        ip_address_range::ip_address_range(const address &addr) {
            if (addr.is_v4()) {
                this->pimpl_ = std::make_shared<detail::single_ipv4_impl>(addr.to_v4());
            } else if (addr.is_v6()) {
                this->pimpl_ = std::make_shared<detail::single_ipv6_impl>(addr.to_v6());
            } else throw std::invalid_argument("address is not an ipv4 or ip v6 address");
        }

        ip_address_range::ip_address_range(const std::string &addr) : ip_address_range(address::from_string(addr)) {}

        ip_address_range::~ip_address_range() {}

        bool ip_address_range::contains(const address &addr) const {
            return this->pimpl_->contains_address(addr);
        }

        bool ip_address_range::contains(const std::string &addr) const {
            return this->pimpl_->contains_address(address::from_string(addr));
        }

        bool ip_address_range::is_v4() const {
            return this->pimpl_->is_v4();
        }

        bool ip_address_range::is_v6() const {
            return pimpl_->is_v6();
        }

        bool ip_address_range::contains(const ip_address_range &range) const {
            return this->pimpl_->contains_range(range.pimpl_.get());
        }

        bool ip_address_range::operator<(const ip_address_range &range) const {
            if (this->first() == range.first()) {
                return this->last() < range.last();
            }
            return this->first() < range.first();
        }

        address ip_address_range::first() const {
            return this->pimpl_->first();
        }

        address ip_address_range::last() const {
            return this->pimpl_->last();
        }

        bool ip_address_range::operator==(const ip_address_range &range) const {
            return this->first() == range.first() &&
                   this->last() == range.last();
        }

        bool ip_address_range::operator==(std::nullptr_t) const {
            return this->pimpl_ == nullptr;
        }

        bool ip_address_range::is_single_address() const {
            return this->pimpl_->first() == this->pimpl_->last();
        }

        ip_address_range::operator address() const {
            if (!this->is_single_address()) {
                throw std::invalid_argument("can't convert address range to a single address");
            }
            return this->pimpl_->first();
        }

        ip_address_range::ip_address_range(const std::shared_ptr<detail::ip_address_range_impl> &impl)
                : pimpl_(impl) {}

        std::string ip_address_range::to_string() const {
            if (this->is_single_address()) return this->first().to_string();
            else {
                auto r = this->first().to_string();
                r.append("-");
                r.append(this->last().to_string());
                return r;
            }
        }

        std::wstring ip_address_range::to_wstring() const {
            auto t = this->to_string();
            return std::wstring(t.begin(), t.end());
        }
    }
}

