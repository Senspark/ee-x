//
//  jsb_ifacebook_bridge.hpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#ifndef EE_X_JSB_I_FACEBOOK_BRIDGE_HPP
#define EE_X_JSB_I_FACEBOOK_BRIDGE_HPP

#include <ee/facebook/FacebookFwd.hpp>

namespace ee {
namespace facebook {
se::Class* getIFacebookBridgeClass();
bool registerJsbIBridge(se::Object* global);
} // namespace facebook
} // namespace ee

#endif /* EE_X_JSB_I_FACEBOOK_BRIDGE_HPP */
