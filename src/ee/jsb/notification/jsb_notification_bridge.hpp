//
//  jsb_notification_bridge.hpp
//  ee_x
//
//  Created by Nguyen Van Quynh on 8/23/18
//

#ifndef JSB_NOTIFICATION_BRIDGE_HPP__
#define JSB_NOTIFICATION_BRIDGE_HPP__

#pragma one

namespace se {
class Object;
} // namespace se

namespace ee {
namespace notification {

bool register_notification_bridge_manual(se::Object* object);

} // namespace notification
} // namespace ee

#endif // JSB_NOTIFICATION_BRIDGE_HPP__
