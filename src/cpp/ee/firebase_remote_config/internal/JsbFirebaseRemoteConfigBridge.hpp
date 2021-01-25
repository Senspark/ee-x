//
//  jsb_firebase_remote_config.hpp
//  ee_x
//
//  Created by Nguyen Van Quynh on 9/23/18
//

#ifndef EE_X_JSB_FIREBASE_REMOTE_CONFIG_BRIDGE_HPP
#define EE_X_JSB_FIREBASE_REMOTE_CONFIG_BRIDGE_HPP

#include <ee/cocos/CocosFwd.hpp>

#ifdef EE_X_COCOS_JS
#include <ee/FirebaseFwd.hpp>

namespace ee {
namespace firebase {
namespace remote_config {
bool registerJsbBridge(se::Object* global);
} // namespace remote_config
} // namespace firebase
} // namespace ee

#endif // EE_X_COCOS_JS

#endif // EE_X_JSB_FIREBASE_REMOTE_CONFIG_BRIDGE_HPP
