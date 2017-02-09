//
// Created by ukheg on 28.09.2016.
//

#ifndef PLAIN_PACKETS_FIELD_SETTER_HPP
#define PLAIN_PACKETS_FIELD_SETTER_HPP
#include "tpl/packets/details/integer_field.hpp"
#include "tpl/packets/details/enum_field.hpp"
#include "tpl/packets/details/byte_sequence_field.hpp"

namespace tpl{
    namespace packets{
        namespace details{
            struct setter_selector_impl {
                template<class t_field_type, size_t t_offset>
                static typename std::enable_if<is_integral_field<t_field_type>::value, integer_setter<t_field_type, t_offset>>::type setter_type();

                template<class t_field_type, size_t t_offset>
                static typename std::enable_if<is_enum_field<t_field_type>::value, enum_setter<t_field_type, t_offset>>::type setter_type();

                template<class t_field_type, size_t t_offset>
                static typename std::enable_if<is_byte_sequence<t_field_type>::value, byte_sequence_setter<t_field_type::length, t_offset>>::type setter_type();
            };


            struct setter_selector{
                template<class t_field_type, size_t t_offset>
                using setter_for = decltype(setter_selector_impl::setter_type<t_field_type, t_offset>());
            };
        }
    }
}
#endif //PLAIN_PACKETS_FIELD_SETTER_HPP
