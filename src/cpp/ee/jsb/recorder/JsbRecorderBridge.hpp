//
//  jsb_notification_bridge.hpp
//  ee_x
//
//  Created by HanPx on 01/12/19.
//

#ifndef EE_X_JSB_RECORDER_BRIDGE_HPP
#define EE_X_JSB_RECORDER_BRIDGE_HPP

#include <ee/jsb/JsbCoreFwd.hpp>

namespace ee {
namespace recorder {
bool registerJsbBridge(se::Object* global);
} // namespace recorder
} // namespace ee

#endif // EE_X_JSB_RECORDER_BRIDGE_HPP
