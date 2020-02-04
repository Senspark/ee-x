//
//  jsb_crashlytics_bridge.hpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/24/18.
//

#ifndef EE_X_JSB_CRASHLYTICS_BRIDGE_HPP
#define EE_X_JSB_CRASHLYTICS_BRIDGE_HPP

#include <ee/jsb/JsbCoreFwd.hpp>

namespace ee {
namespace crashlytics {
bool registerJsbBridge(se::Object* global);
} // namespace crashlytics
} // namespace ee

#endif //EE_X_JSB_CRASHLYTICS_BRIDGE_HPP
