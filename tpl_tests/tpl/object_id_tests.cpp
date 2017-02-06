//
// Created by ukhegg on 06.11.2016.
//

#include <gtest/gtest.h>
#include <tpl/object_id.hpp>

using namespace tpl;

namespace tpl_tests {
    TEST(object_id_tests, can_register_object_id) {
        auto id = objects_id_repository::instance().register_type(objects_id_repository::instance().root_object_id(),
                                                                  "some node");
    }
}