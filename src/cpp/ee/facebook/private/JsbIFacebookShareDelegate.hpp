//
//  jsb_ifacebook_share_delegate.hpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#ifndef EE_X_JSB_I_FACEBOOK_SHARE_DELEGATE_HPP
#define EE_X_JSB_I_FACEBOOK_SHARE_DELEGATE_HPP

#include <ee/cocos/CocosFwd.hpp>

#ifdef EE_X_COCOS_JS
#include "ee/facebook/FacebookFwd.hpp"

namespace ee {
namespace facebook {
bool registerJsbIShareDelegate(se::Object* global);
} // namespace facebook
} // namespace ee

#endif // EE_X_COCOS_JS

#endif /* EE_X_JSB_I_FACEBOOK_SHARE_DELEGATE_HPP */
