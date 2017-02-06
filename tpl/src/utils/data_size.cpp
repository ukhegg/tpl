#include <tpl/utils/data_size.hpp>
#include <sstream>
#include <boost/locale/encoding_utf.hpp>
#include <limits>

namespace tpl {
    namespace utils {
	    data_size data_size::bytes(uint64_t count)
	    {
			return data_size(count * bytes_in<data_size_unit::byte>::count);
	    }

	    data_size data_size::kilobytes(uint64_t count)
	    {
			return data_size(count * bytes_in<data_size_unit::kilobyte>::count);
	    }

	    data_size data_size::megabytes(uint64_t count)
	    {
			return  data_size(count * bytes_in<data_size_unit::megabyte>::count);
	    }

	    data_size data_size::gigabytes(uint64_t count)
	    {
			return  data_size(count * bytes_in<data_size_unit::gigabyte>::count);
	    }

	    data_size data_size::terabytes(uint64_t count)
	    {
			return  data_size(count * bytes_in<data_size_unit::terabyte>::count);
	    }

	    data_size::data_size() {}

        data_size::data_size(int64_t total_bytes)
                : total_bytes_(total_bytes) {}

        data_size::data_size(double count, data_size_unit units) {
            switch (units) {
                case data_size_unit::byte:
                    this->total_bytes_ = static_cast<uint64_t>(count * bytes_in<data_size_unit::byte>::count);
                    break;
                case data_size_unit::kilobyte:
                    this->total_bytes_ = static_cast<uint64_t>(count * bytes_in<data_size_unit::kilobyte>::count);
                    break;
                case data_size_unit::megabyte:
                    this->total_bytes_ = static_cast<uint64_t>(count * bytes_in<data_size_unit::megabyte>::count);
                    break;
                case data_size_unit::gigabyte:
                    this->total_bytes_ = static_cast<uint64_t>(count * bytes_in<data_size_unit::gigabyte>::count);
                    break;
                case data_size_unit::terabyte:
                    this->total_bytes_ = static_cast<uint64_t>(count * bytes_in<data_size_unit::terabyte>::count);
                    break;
                default:
                    throw std::logic_error("invalid unit argument");
            }
        }

        data_size::~data_size() {}

        data_size data_size::operator+(int64_t rhv) const {
            return data_size(this->total_bytes_ + rhv);
        }

        data_size data_size::operator-(int64_t rhv) const {
            return data_size(this->total_bytes_ - rhv);
        }

        data_size data_size::operator/(double div) const {
            return data_size(static_cast<uint64_t>(this->total_bytes_ / div));
        }

        data_size data_size::operator*(double mul) const {
            return data_size(static_cast<uint64_t>(this->total_bytes_ * mul));
        }

        data_size data_size::operator+(data_size rhv) const {
            return data_size(this->total_bytes_ + rhv.total_bytes_);
        }

        data_size data_size::operator-(data_size rhv) const {
            return data_size(this->total_bytes_ - rhv.total_bytes_);
        }

        double data_size::operator/(data_size rhv) const {
            return static_cast<double>(this->total_bytes_) / rhv.total_bytes_;
        }

        int64_t data_size::total(data_size_unit unit) const {
            switch (unit) {
                case data_size_unit::byte:
                    return this->total<data_size_unit::byte>();
                case data_size_unit::kilobyte:
                    return this->total<data_size_unit::kilobyte>();
                case data_size_unit::megabyte:
                    return this->total<data_size_unit::megabyte>();
                case data_size_unit::gigabyte:
                    return this->total<data_size_unit::gigabyte>();
                case data_size_unit::terabyte:
                    return this->total<data_size_unit::terabyte>();
                default:
                    throw std::logic_error("invalid unit argument");
            }
        }

        int64_t data_size::count(data_size_unit unit) const {
            switch (unit) {
                case data_size_unit::byte:
                    return this->count<data_size_unit::byte>();
                case data_size_unit::kilobyte:
                    return this->count<data_size_unit::kilobyte>();
                case data_size_unit::megabyte:
                    return this->count<data_size_unit::megabyte>();
                case data_size_unit::gigabyte:
                    return this->count<data_size_unit::gigabyte>();
                case data_size_unit::terabyte:
                    return this->count<data_size_unit::terabyte>();
                default:
                    throw std::logic_error("invalid unit argument");
            }
        }

        data_size_unit data_size::order() const {
            if (*this >= terabytes(1)) return data_size_unit::terabyte;
            if (*this >= gigabytes(1)) return data_size_unit::gigabyte;
            if (*this >= megabytes(1)) return data_size_unit::megabyte;
            if (*this >= kilobytes(1)) return data_size_unit::kilobyte;
            return data_size_unit::byte;
        }

        data_size &data_size::operator+=(const data_size &rhv) {
            this->total_bytes_ += rhv.total_bytes_;
            return *this;
        }

        data_size &data_size::operator+=(int64_t rhv) {
            this->total_bytes_ += rhv;
            return *this;
        }

        data_size &data_size::operator-=(const data_size &rhv) {
            this->total_bytes_ -= rhv.total_bytes_;
            return *this;
        }

        data_size &data_size::operator-=(int64_t rhv) {
            this->total_bytes_ -= rhv;
            return *this;
        }

        data_size &data_size::operator*=(double mul) {
            this->total_bytes_ = static_cast<uint64_t>(this->total_bytes_ * mul);
            return *this;
        }

        data_size &data_size::operator/=(double div) {
            this->total_bytes_ = static_cast<uint64_t>(this->total_bytes_ / div);
            return *this;
        }

        std::string data_size::string_helper::to_string(data_size const &ds, size_t max_digits, char) {
            if (max_digits >= 64) {
                max_digits = 64;
            }

            if (ds.total_bytes_ == 0) {
                return "0 B";
            }

            auto size_order = ds.order();

            auto order_units_count = ds / data_size(1, size_order);
            char buffer[128];

            int dot_position, sign;
			if(max_digits > std::numeric_limits<int>::max())
			{
				throw std::invalid_argument("too many digits");
			}
            _ecvt_s(buffer, sizeof(buffer), order_units_count, 
				static_cast<int>(max_digits), &dot_position, &sign);

            auto chars_left_to_fill = max_digits;
            std::string res;
            res.assign(buffer, buffer + dot_position);

            auto dot_inserted = false;

            if (size_order != data_size_unit::byte) {
                chars_left_to_fill -= dot_position;

                if (chars_left_to_fill > 1) {
                    res.push_back('.');
                    dot_inserted = true;

                    chars_left_to_fill--;

                    std::copy(buffer + dot_position,
                              buffer + dot_position + chars_left_to_fill,
                              std::back_inserter(res));
                }
            }

            while (dot_inserted && res.back() == '0') res.pop_back();
            if (res.back() == '.') res.pop_back();

            res.push_back(' ');
            res.append(detail::string_helper::to_string(size_order, char()));
            return res;
        }

        std::string detail::string_helper::to_string(data_size_unit unit, char) {
            switch (unit) {
                case data_size_unit::byte:
                    return "B";
                case data_size_unit::kilobyte:
                    return "Kb";
                case data_size_unit::megabyte:
                    return "Mb";
                case data_size_unit::gigabyte:
                    return "Gb";
                case data_size_unit::terabyte:
                    return "Tb";
                default:
                    throw std::logic_error("invalid unit argument");
            }
        }

        std::wstring detail::string_helper::to_string(data_size_unit unit, wchar_t) {
            return boost::locale::conv::utf_to_utf<wchar_t>(to_string(unit, char()));
        }

        std::wstring data_size::string_helper::to_string(data_size const &ds, size_t max_digits, wchar_t) {
            return boost::locale::conv::utf_to_utf<wchar_t>(to_string(ds, max_digits, char()));
        }

        bool data_size::operator==(const data_size &rhv) const {
            return this->total_bytes_ == rhv.total_bytes_;
        }

        bool data_size::operator!=(const data_size &rhv) const {
            return this->total_bytes_ != rhv.total_bytes_;
        }

        bool data_size::operator<(const data_size &rhv) const {
            return this->total_bytes_ < rhv.total_bytes_;
        }

        bool data_size::operator<=(const data_size &rhv) const {
            return this->total_bytes_ <= rhv.total_bytes_;
        }

        bool data_size::operator>(const data_size &rhv) const {
            return this->total_bytes_ > rhv.total_bytes_;
        }

        bool data_size::operator>=(const data_size &rhv) const {
            return this->total_bytes_ >= rhv.total_bytes_;
        }

        bool data_size::operator<(long long rhv) const {
            return this->total_bytes_ < rhv;
        }

        bool data_size::operator<=(long long rhv) const {
            return this->total_bytes_ < rhv;
        }

        bool data_size::operator>(long long rhv) const {
            return this->total_bytes_ > rhv;
        }

        bool data_size::operator>=(long long rhv) const {
            return this->total_bytes_ > rhv;
        }

        data_size operator+(int64_t lhv, data_size const &ds) {
            return data_size(ds.total_bytes_ + lhv);
        }

        data_size operator-(int64_t rhv, data_size const &ds) {
            return data_size(ds.total_bytes_ - rhv);
        }

        data_size operator*(double mul, data_size const &ds) {
            return data_size(static_cast<uint64_t>(ds.total_bytes_ * mul));
        }
    }
}

