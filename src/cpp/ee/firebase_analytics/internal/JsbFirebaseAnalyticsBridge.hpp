//
//  jsb_crashlytics_bridge.hpp
//  ee_x
//
//  Created by Nguyen Van Quynh on 8/22/18
//

#ifndef EE_X_JSB_FIREBASE_ANALYTICS_BRIDGE_HPP
#define EE_X_JSB_FIREBASE_ANALYTICS_BRIDGE_HPP

#include <ee/cocos/CocosFwd.hpp>

#ifdef EE_X_COCOS_JS

#include <ee/FirebaseFwd.hpp>

namespace ee {
namespace firebase {
namespace analytics {
bool registerJsbBridge(se::Object* global);
} // namespace analytics
} // namespace firebase
} // namespace ee

#endif // EE_X_COCOS_JS

#endif // EE_X_JSB_FIREBASE_ANALYTICS_BRIDGE_HPP
