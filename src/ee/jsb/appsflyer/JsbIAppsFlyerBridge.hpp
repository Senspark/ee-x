//
//  JsbIAppsFlyerBridge.hpp
//  ee-x
//
//  Created by eps on 12/05/19.
//

#ifndef EE_X_JSB_I_APPS_FLYER_HPP
#define EE_X_JSB_I_APPS_FLYER_HPP

#include "ee/jsb/jsb_fwd.hpp"

namespace ee {
namespace appsflyer {
se::Class* getIBridgeClass();
bool registerJsbIBridge(se::Object* global);
} // namespace appsflyer
} // namespace ee

#endif /* EE_X_JSB_I_APPS_FLYER_HPP */
