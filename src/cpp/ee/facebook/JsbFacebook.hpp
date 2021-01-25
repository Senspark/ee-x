//
//  jsb_facebook.hpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#ifndef EE_X_JSB_FACEBOOK_HPP
#define EE_X_JSB_FACEBOOK_HPP

#ifdef __cplusplus

#include <ee/cocos/CocosFwd.hpp>

#ifdef EE_X_COCOS_JS
#include "ee/facebook/FacebookFwd.hpp"

namespace ee {
namespace facebook {
bool registerJsb(se::Object* global);
} // namespace facebook
} // namespace ee

#endif // EE_X_COCOS_JS
#endif // __cplusplus

#endif /* EE_X_JSB_FACEBOOK_HPP */
