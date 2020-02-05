//
//  jsb_notification.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/JsbNotification.hpp"

#include "ee/jsb/notification/JsbNotificationBridge.hpp"
#include "ee/jsb/notification/JsbNotificationBuilder.hpp"

namespace ee {
namespace notification {
bool registerJsb(se::Object* global) {
    registerJsbBridge(global);
    registerJsbBuilder(global);
    return true;
}
} // namespace notification
} // namespace ee
