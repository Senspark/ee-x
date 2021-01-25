//
//  jsb_facebook_request_content.hpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#ifndef EE_X_JSB_FACEBOOK_REQUEST_CONTENT_HPP
#define EE_X_JSB_FACEBOOK_REQUEST_CONTENT_HPP

#include <ee/cocos/Cocos.hpp>

#ifdef EE_X_COCOS_JS
#include <ee/facebook/FacebookFwd.hpp>

namespace ee {
namespace facebook {
bool registerJsbRequestContent(se::Object* global);
} // namespace facebook
} // namespace ee

#endif // EE_X_COCOS_JS

#endif /* EE_X_JSB_FACEBOOK_REQUEST_CONTENT_HPP */
