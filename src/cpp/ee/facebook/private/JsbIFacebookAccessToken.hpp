//
//  jsb_ifacebook_access_token.hpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#ifndef EE_X_JSB_I_FACEBOOK_ACCESS_TOKEN_HPP
#define EE_X_JSB_I_FACEBOOK_ACCESS_TOKEN_HPP

#include <ee/cocos/CocosFwd.hpp>

#ifdef EE_X_COCOS_JS
#include "ee/facebook/FacebookFwd.hpp"

namespace ee {
namespace facebook {
se::Class* getIAccessTokenClass();
bool registerJsbIAccessToken(se::Object* global);
} // namespace facebook
} // namespace ee

#endif // EE_X_COCOS_JS

#endif /* EE_X_JSB_I_FACEBOOK_ACCESS_TOKEN_HPP */
