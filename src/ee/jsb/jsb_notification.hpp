//
//  jsb_notification.hpp
//  ee_x
//
//  Created by Nguyen Van Quynh on 8/23/18
//

#ifndef JSB_NOTIFICATION_HPP__
#define JSB_NOTIFICATION_HPP__

#ifdef __cplusplus
#include "ee/jsb/notification/jsb_notification_builder.hpp"
#include "ee/jsb/notification/jsb_notification_bridge.hpp"

namespace ee {
namespace notification {
bool register_all_notification_manual(se::Object* object) {
    register_notification_builder_manual(object);
    register_notification_bridge_manual(object);
    return true;
} // namespace register_all_notification_manual
} // namespace notification
} // namespace ee

#endif // __cplusplus
#endif // JSB_NOTIFICATION_HPP__
