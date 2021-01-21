//
//  jsb_facebook_bridge_imp.hpp
//  ee-x
//
//  Created by senspark-dev on 12/13/18.
//

#ifndef EE_X_JSB_FACEBOOK_BRIDGE_HPP
#define EE_X_JSB_FACEBOOK_BRIDGE_HPP

#include <ee/cocos/Cocos.hpp>

#ifdef EE_X_COCOS_JS
#include "ee/facebook/FacebookFwd.hpp"

namespace ee {
namespace facebook {
bool registerJsbBridge(se::Object* global);
} // namespace facebook
} // namespace ee

#endif // EE_X_COCOS_JS

#endif /* EE_X_JSB_FACEBOOK_BRIDGE_HPP */
