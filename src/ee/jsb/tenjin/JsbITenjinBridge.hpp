//
//  Created by eps on 12/6/19.
//

#ifndef EE_X_JSB_I_TENJIN_BRIDGE_HPP
#define EE_X_JSB_I_TENJIN_BRIDGE_HPP

#include <ee/jsb/JsbCoreFwd.hpp>

namespace ee {
namespace tenjin {
se::Class* getIBridgeClass();
bool registerJsbIBridge(se::Object* global);
} // namespace tenjin
} // namespace ee

#endif /* EE_X_JSB_I_TENJIN_BRIDGE_HPP */
