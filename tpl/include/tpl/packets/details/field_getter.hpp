//
// Created by ukheg on 28.09.2016.
//

#ifndef PLAIN_PACKETS_FIELD_GETTER_HPP
#define PLAIN_PACKETS_FIELD_GETTER_HPP

#include "integer_field.hpp"
#include "enum_field.hpp"
#include "byte_sequence_field.hpp"

namespace tpl{
    namespace packets{
        namespace details{
            struct getter_selector_impl {
                template<class t_field_type, size_t t_offset>
                static typename std::enable_if<is_integral_field<t_field_type>::value, integer_getter<t_field_type, t_offset>>::type getter_type();

                template<class t_field_type, size_t t_offset>
                static typename std::enable_if<is_enum_field<t_field_type>::value, enum_getter<t_field_type, t_offset>>::type getter_type();

                template<class t_field_type, size_t t_offset>
                static typename std::enable_if<is_byte_sequence<t_field_type>::value, byte_sequence_getter<t_field_type::length, t_offset>>::type getter_type();
            };


            struct getter_selector{
                template<class t_field_type, size_t t_offset>
                using getter_for = decltype(getter_selector_impl::getter_type<t_field_type, t_offset>());
            };
        }
    }
}
#endif //PLAIN_PACKETS_FIELD_GETTER_HPP
