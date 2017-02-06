//
// Created by ukheg on 27.09.2016.
//

#ifndef PLAIN_PACKETS_D_ARRAY_HPP
#define PLAIN_PACKETS_D_ARRAY_HPP

#include <vector>
#include <memory> // TR1 cyclic inclusion fix


#include <stdexcept>
#include <algorithm>
#include <boost/smart_ptr/shared_array.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/detail/shared_count.hpp>
#include <boost/smart_ptr/detail/sp_nullptr_t.hpp>
#include <boost/container/small_vector.hpp>
#include <boost/detail/workaround.hpp>
#include <initializer_list>
#include <cstddef> // for std::size_t
#include <algorithm> // for std::swap
#include <functional> // for std::less
#include <boost/format.hpp>

namespace tpl {
    namespace utils {
        //
        // class d_array
        //
        // d_array is a shared_array with moving pointer to first element
        // so you can create several d_array manipulating different
        // parts of the same dynamic allocated native array
        // given this,you can't resize d_array,only recreate
        template<class t_element>
        class d_array {
        private:
            // Borland 5.5.1 specific workarounds
            typedef boost::checked_array_deleter<t_element> deleter;
            typedef d_array<t_element> this_type;
        public:
            typedef t_element element_type;

            d_array() BOOST_NOEXCEPT : px_(nullptr), pn_(), size_(0) {}

            explicit d_array(size_t size) : px_(new t_element[size]), pn_(), size_(size) {
                pn_ = boost::detail::shared_count(px_, deleter());
            }

            d_array(const std::initializer_list<t_element> &l)
                    : px_(new t_element[l.size()]),
                      pn_(),
                      size_(l.size()) {
                pn_ = boost::detail::shared_count(px_, deleter());
                std::copy(l.begin(), l.end(), this->begin());
            }

            d_array(t_element *p, size_t size) : px_(p), size_(size), pn_(p, deleter()) {
            }

            template<class Y>
            explicit d_array(Y *p, size_t size) : px_(p), size_(size), pn_(p, boost::checked_array_deleter<Y>()) {
                boost::detail::sp_assert_convertible<Y[], t_element[]>();
            }

            //
            // Requirements: D's copy constructor must not throw
            //
            // shared_array will release p by calling d(p)
            //
            template<class Y, class D>
            d_array(Y *p, D d, size_t size)
                    : px_(p), size_(size), pn_(p, d) {
                boost::detail::sp_assert_convertible<Y[], t_element[]>();
            }

            // As above, but with allocator. A's copy constructor shall not throw.
            template<class Y, class D, class A>
            d_array(Y *p, D d, A a, size_t size)
                    : px_(p), size_(size), pn_(p, d, a) {
                boost::detail::sp_assert_convertible<Y[], t_element[]>();
            }

            //  generated copy constructor, destructor are fine...
#if !defined( BOOST_NO_CXX11_RVALUE_REFERENCES )

            // ... except in C++0x, move disables the implicit copy
            d_array(d_array const &r) BOOST_NOEXCEPT : px_(r.px_), pn_(r.pn_), size_(r.size_) {
            }

            d_array(d_array &&r) BOOST_NOEXCEPT : px_(r.px_), pn_(), size_(r.size_) {
                pn_.swap(r.pn_);
                r.px_ = 0;
            }

#endif

            ~d_array() {}

            // assignment
            d_array &operator=(d_array const &r) BOOST_NOEXCEPT {
                this_type(r).swap(*this);
                this->size_ = r.size_;
                return *this;
            }

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1400)

            template<class Y>
            d_array &operator=(d_array<Y> const &r) BOOST_NOEXCEPT {
                this_type(r).swap(*this);
                this->size_ = r.size_;
                return *this;
            }

#endif
#if !defined( BOOST_NO_CXX11_RVALUE_REFERENCES )

            d_array &operator=(d_array &&r) BOOST_NOEXCEPT {
                this_type(static_cast<d_array &&>(r)).swap(*this);
                this->size_ = r.size_;
                return *this;
            }

            template<class Y>
            d_array &operator=(d_array<Y> &&r) BOOST_NOEXCEPT {
                this_type(static_cast<d_array<Y> &&>(r)).swap(*this);
                this->size_ = r.size_;
                return *this;
            }

#endif

            void reset() BOOST_NOEXCEPT {
                this_type().swap(*this);
            }

            template<class Y>
            void reset(Y *p) // Y must be complete
            {
                BOOST_ASSERT(p == 0 || p != px_); // catch self-reset errors
                this_type(p).swap(*this);
            }

            template<class Y, class D>
            void reset(Y *p, D d) {
                this_type(p, d).swap(*this);
            }

            template<class Y, class D, class A>
            void reset(Y *p, D d, A a) {
                this_type(p, d, a).swap(*this);
            }

            template<class Y>
            void reset(d_array<Y> const &r, element_type *p) {
                this_type(r, p).swap(*this);
            }

            const t_element &operator[](std::size_t i) const {
                BOOST_ASSERT(px_ != 0);
                if (i >= this->size_) {
                    auto message =
                            boost::format("d_array<T>::operator[](ptrdifff_t) const error:index(%d) > this->size_(%d)")
                            % i
                            % this->size_;
                    throw std::out_of_range(message.str());
                }
                return px_[i];
            }

            t_element &operator[](std::size_t i) {
                BOOST_ASSERT(px_ != 0);
                if (i >= this->size_) {
                    auto message = boost::format("d_array<T>::operator[](ptrdifff_t) error:index(%d) > this->size_(%d)")
                                   % i
                                   % this->size_;
                    throw std::out_of_range(message.str());
                }
                return px_[i];
            }

            t_element *begin() {
                return px_;
            }

            const t_element *begin() const {
                return px_;
            }

            t_element *end() {
                return px_ + size_;
            }

            const t_element *end() const {
                return px_ + size_;
            }

            bool unique() const BOOST_NOEXCEPT {
                return pn_.unique();
            }

            bool is_empty() const BOOST_NOEXCEPT {
                return this->size_ == 0;
            }

            long use_count() const BOOST_NOEXCEPT {
                return pn_.use_count();
            }

            void swap(d_array<t_element> &other) BOOST_NOEXCEPT {
                std::swap(this->px_, other.px_);
                pn_.swap(other.pn_);
                std::swap(this->size_, other.size_);
            }

            t_element *data() BOOST_NOEXCEPT {
                return this->px_;
            }

            const t_element *data() const BOOST_NOEXCEPT {
                return this->px_;
            }

            size_t size() const BOOST_NOEXCEPT {
                return this->size_;
            }

            bool operator==(const d_array &rhv) const {
                if (this->size_ != rhv.size_) return false;

                for (size_t index = 0; index < this->size_; ++index) {
                    if ((*this)[index] != rhv[index]) return false;
                }

                return true;
            }

            //////////////////////////////////////////////////////////////////////////
            // prefix(size_t) - get's prefix of array
            // length - length of the prefix
            //////////////////////////////////////////////////////////////////////////
            d_array prefix(size_t length) const {
                if (length > this->size_) {
                    auto message = boost::format("d_array<T>::middle error:length(%d) > size(%d)")
                                   % length
                                   % this->size_;
                    throw std::out_of_range(message.str());
                }
                return std::move(d_array(*this, 0, length));
            }

            //////////////////////////////////////////////////////////////////////////
            // gets some elements in the middle of array
            //////////////////////////////////////////////////////////////////////////
            d_array middle(size_t offset, size_t length) const {
                if (offset > this->size_ ||
                    length > this->size_ ||
                    offset + length > this->size_) {
                    auto message = boost::format("d_array<T>::middle error:offset(%d) + length(%d) > size(%d)")
                                   % offset
                                   % length
                                   % this->size_;
                    throw std::out_of_range(message.str());
                }
                return std::move(d_array(*this, offset, length));
            }

            //////////////////////////////////////////////////////////////////////////
            // gets suffix of array starting from offset
            //////////////////////////////////////////////////////////////////////////
            d_array suffix(size_t offset) const {
                if (offset > this->size_) {
                    auto message = boost::format("d_array<T>::middle error:offset(%d)> size(%d)")
                                   % offset
                                   % this->size_;
                    throw std::out_of_range(message.str());
                }
                return std::move(d_array(*this, offset, this->size_ - offset));
            }

            //////////////////////////////////////////////////////////////////////////
            // creates a deep copy of this
            //////////////////////////////////////////////////////////////////////////
            d_array clone() const {
                d_array result(this->size_);
                memcpy_s(result.begin(), result.size() * sizeof(t_element), this->begin(),
                         this->size_ * sizeof(t_element));
                //std::copy(this->begin(), this->end(), result.begin());
                return result;
            }

            void copy_to(std::vector<t_element> &v) const {
                v.resize(this->size_);
                this->copy_to(v.begin());
            }

            template<class t_iterator>
            t_iterator copy_to(const t_iterator &insert_iterator) const {
                return std::copy(this->begin(), this->end(), insert_iterator);
            }

            bool contains(const d_array &rhv) const {
                return rhv.begin() >= this->begin() && rhv.end() <= this->end();
            }

            operator std::vector<t_element>() const {
                return std::vector<t_element>(this->begin(), this->end());
            }

            typedef struct merge_proxy_ {
                merge_proxy_() {}

                explicit merge_proxy_(d_array const &initial)
                        : target_size_(initial.size()), merging_({initial}) {}

                merge_proxy_(const d_array &first, const d_array &second)
                        : target_size_(first.size() + second.size()), merging_({first, second}) {}

                merge_proxy_ &merge(const d_array &rhv) {
                    this->merging_.push_back(rhv);
                    target_size_ += rhv.size();
                    return *this;
                }

                d_array flush() const {
                    if (this->merging_.empty()) return d_array();


                    d_array result(this->target_size_);
                    auto copy_pos = result.begin();
                    for (const d_array &src : this->merging_) {
                        copy_pos = src.copy_to(copy_pos);
                    }
                    return result;
                }

                operator d_array() {
                    return this->flush();
                }

                size_t size() const {
                    return this->target_size_;
                }

                void clear() {
                    this->merging_.clear();
                    this->target_size_ = 0;
                }

                d_array prepend_flush(d_array const &arr) {
                    d_array result(this->target_size_ + arr.size());

                    arr.copy_to(result.begin());
                    size_t copy_offset = arr.size();

                    for (const d_array &src : this->merging_) {
                        src.copy_to(result.begin() + copy_offset);
                        copy_offset += src.size();
                    }
                    this->clear();
                    return result;
                }

                void prepend(d_array const &arr) {
                    this->merging_.push_front(arr);
                    this->target_size_ += arr.size();
                }

            private:
                size_t target_size_{0};
                boost::container::small_vector<d_array, 5> merging_;
            } merge_proxy;

            merge_proxy_ merge(const d_array &rhv) const {
                return merge_proxy_(*this, rhv);
            }

            static d_array merge(d_array const &first, d_array const &second) {
                auto res = d_array(first.size_ + second.size_);
                auto second_copy_pos = first.copy_to(res.begin());
                second.copy_to(second_copy_pos);
                return res;
            }

        private:
            t_element *px_; // pointer to this array's first element
            boost::detail::shared_count pn_;
            size_t size_;

            d_array(const d_array &super, size_t offset, size_t length)
                    : px_(super.px_ + offset), pn_(super.pn_), size_(length) {
            }
        };
    }

    typedef utils::d_array<uint8_t> byte_array;
}

namespace std
{
	template<class t_element>
	struct less<tpl::utils::d_array<t_element>>{
		bool operator()(tpl::utils::d_array<t_element> const& lhs, tpl::utils::d_array<t_element> const& rhs) const{
			if (lhs.size() != rhs.size()) return  lhs.size() < rhs.size();

			for(size_t index = 0; index <  lhs.size(); ++index)
			{
				if (lhs[index] < rhs[index]) return  true;
				if (rhs[index] < lhs[index]) return  false;
			}

			return false;
		}
	};
}
#endif //PLAIN_PACKETS_D_ARRAY_HPP
