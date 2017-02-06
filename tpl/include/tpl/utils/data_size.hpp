#pragma once

#include <tpl/tpl.hpp>
#include <cinttypes>
#include <ostream>

namespace tpl {
    namespace utils {
        enum class data_size_unit {
            byte,
            kilobyte,
            megabyte,
            gigabyte,
            terabyte
        };

        template<data_size_unit unit>
        struct bytes_in {
        };

        template<>
        struct bytes_in<data_size_unit::byte> {
            static const int64_t count = 1;
        };

        template<>
        struct bytes_in<data_size_unit::kilobyte> {
            static const int64_t count = 1024 * bytes_in<data_size_unit::byte>::count;
        };

        template<>
        struct bytes_in<data_size_unit::megabyte> {
            static const int64_t count = 1024 * bytes_in<data_size_unit::kilobyte>::count;
        };

        template<>
        struct bytes_in<data_size_unit::gigabyte> {
            static const int64_t count = 1024 * bytes_in<data_size_unit::megabyte>::count;
        };

        template<>
        struct bytes_in<data_size_unit::terabyte> {
            static const int64_t count = 1024 * bytes_in<data_size_unit::gigabyte>::count;
        };

        class TPL_API data_size {
        public:
            static data_size bytes(uint64_t count);
            static data_size kilobytes(uint64_t count);
            static data_size megabytes(uint64_t count);
            static data_size gigabytes(uint64_t count);
            static data_size terabytes(uint64_t count);

            data_size();

            explicit data_size(int64_t total_bytes);

            data_size(double count, data_size_unit units);

            ~data_size();

            data_size operator+(int64_t bytes) const;

            data_size operator-(int64_t bytes) const;

            data_size operator/(double div) const;

            data_size operator*(double mul) const;

            data_size operator+(data_size bytes) const;

            data_size operator-(data_size bytes) const;

            data_size &operator+=(const data_size &data_size);

            data_size &operator+=(int64_t data_size);

            data_size &operator-=(const data_size &data_size);

            data_size &operator-=(int64_t data_size);

            data_size &operator*=(double mul);

            data_size &operator/=(double div);

            double operator/(data_size bytes) const;

            bool operator==(const data_size &rhv) const;

            bool operator!=(const data_size &rhv) const;

            bool operator<(const data_size &rhv) const;

            bool operator<=(const data_size &rhv) const;

            bool operator>(const data_size &rhv) const;

            bool operator>=(const data_size &rhv) const;

            bool operator<(__int64 rhv) const;

            bool operator<=(__int64 rhv) const;

            bool operator>(__int64 rhv) const;

            bool operator>=(__int64 rhv) const;

            template<data_size_unit unit>
            int64_t total() const {
                return this->total_bytes_ / bytes_in<unit>::count;
            }

            int64_t total(data_size_unit unit) const;

            template<data_size_unit unit>
            int64_t count() const {
                return (this->total_bytes_ / bytes_in<unit>::count) % 1024;
            }

            int64_t count(data_size_unit unit) const;

            data_size_unit order() const;

            friend data_size operator+(int64_t bytes, data_size const &ds);

            friend data_size operator-(int64_t bytes, data_size const &ds);

            friend data_size operator*(double mul, data_size const &ds);

            template<class t_char>
            std::basic_string<t_char> to_string() const {
                static_assert(std::is_same<t_char, char>::value ||
                              std::is_same<t_char, wchar_t>::value,
                              "t_char must be either char or wchar_t");

                return string_helper::to_string(*this, 4, t_char());
            }


            template<class t_char>
            friend std::basic_ostream<t_char> &operator<<(std::basic_ostream<t_char> &stream, const data_size &ds) {
                return stream << ds.to_string<t_char>();
            }

        private:
            int64_t total_bytes_{0};

            struct TPL_API string_helper {
                static std::string to_string(data_size const &ds, size_t max_digits, char);

                static std::wstring to_string(data_size const &ds, size_t max_digits, wchar_t);
            };
        };

        namespace detail {
            struct TPL_API string_helper {
                static std::string to_string(data_size_unit unit, char);

                static std::wstring to_string(data_size_unit unit, wchar_t);
            };
        }

        template<class t_char>
        std::basic_ostream<t_char> &operator<<(std::basic_ostream<t_char> &stream, data_size_unit unit) {
            return stream << detail::string_helper::to_string(unit, t_char());
        }
    }
}

