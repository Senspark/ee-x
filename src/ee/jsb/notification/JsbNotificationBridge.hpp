//
//  jsb_notification_bridge.hpp
//  ee_x
//
//  Created by Nguyen Van Quynh on 8/23/18
//

#ifndef EE_X_JSB_NOTIFICATION_BRIDGE_HPP
#define EE_X_JSB_NOTIFICATION_BRIDGE_HPP

#include <ee/jsb/JsbCoreFwd.hpp>

namespace ee {
namespace notification {
bool registerJsbBridge(se::Object* global);
} // namespace notification
} // namespace ee

#endif // EE_X_JSB_NOTIFICATION_BRIDGE_HPP
