//
//  JsbIAppsFlyerBridge.hpp
//  ee-x
//
//  Created by eps on 12/05/19.
//

#ifndef EE_X_JSB_I_APPS_FLYER_BRIDGE_HPP
#define EE_X_JSB_I_APPS_FLYER_BRIDGE_HPP

#include <ee/jsb/JsbCoreFwd.hpp>

namespace ee {
namespace apps_flyer {
se::Class* getIBridgeClass();
bool registerJsbIBridge(se::Object* global);
} // namespace apps_flyer
} // namespace ee

#endif /* EE_X_JSB_I_APPS_FLYER_BRIDGE_HPP */
