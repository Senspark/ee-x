//
//  jsb_notification.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/jsb_notification.hpp"

#include "ee/jsb/notification/jsb_notification_bridge.hpp"
#include "ee/jsb/notification/jsb_notification_builder.hpp"

namespace ee {
namespace notification {
bool register_all_notification_manual(se::Object* object) {
    register_notification_builder_manual(object);
    register_notification_bridge_manual(object);
    return true;
}
} // namespace notification
} // namespace ee
