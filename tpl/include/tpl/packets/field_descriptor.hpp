//
// Created by ukheg on 27.09.2016.
//

#ifndef PLAIN_PACKETS_FIELD_DESCRYPTOR_HPP
#define PLAIN_PACKETS_FIELD_DESCRYPTOR_HPP

#include <type_traits>
#include "tpl/packets/details/field_getter.hpp"
#include "tpl/packets/details/field_setter.hpp"

namespace tpl {
    namespace packets {
        template<class t_field_type, class t_previous_field = void>
        struct field_descriptor :
		        details::getter_selector::getter_for<t_field_type, t_previous_field::position + t_previous_field::size>,
		        details::setter_selector::setter_for<t_field_type, t_previous_field::position + t_previous_field::size> {
	        enum : size_t {
                position = t_previous_field::position + t_previous_field::size,
                size = sizeof(t_field_type)
            };

            using getter_type = details::getter_selector::getter_for<t_field_type, t_previous_field::position + t_previous_field::size>;

            using setter_type = details::setter_selector::setter_for<t_field_type, t_previous_field::position + t_previous_field::size>;
        };

        template<class t_field_type>
        struct field_descriptor<t_field_type, void> :
		        details::getter_selector::getter_for<t_field_type, 0>,
		        details::setter_selector::setter_for<t_field_type, 0> {
	        enum : size_t{
                position = 0,
                size = sizeof(t_field_type)
            };

            using getter_type = details::getter_selector::getter_for<t_field_type, 0>;

            using setter_type = details::setter_selector::setter_for<t_field_type, 0>;
        };
    }
}
#endif //PLAIN_PACKETS_FIELD_DESCRYPTOR_HPP
