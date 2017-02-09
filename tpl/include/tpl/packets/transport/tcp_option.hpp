#pragma once

#include "tpl/tpl.hpp"
#include "tpl/utils/d_array.hpp"
#include <stdint.h>

namespace tpl {
    namespace packets {
        class TPL_API tcp_option {
        public:
            enum class option_types : uint8_t {
                end_of_options = 0,
                no_operation = 1,
                maximum_segment_size = 2,
                window_scale = 3,
                sack_permitted_option = 4,
                timestamp = 8
            };

            explicit tcp_option(const byte_array &bytes);

            ~tcp_option();

            option_types type() const;

            uint8_t length() const;

            const byte_array &bytes() const;

            static uint8_t option_size(byte_array const& bytes);

        private:
            byte_array bytes_;
        };
    }
}

