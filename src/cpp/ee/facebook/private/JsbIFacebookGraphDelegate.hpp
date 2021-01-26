//
//  jsb_ifacebook_graph_delegate.hpp
//  Pods
//
//  Created by senspark-dev on 1/8/19.
//

#ifndef EE_X_JSB_I_FACEBOOK_GRAPH_DELEGATE_HPP
#define EE_X_JSB_I_FACEBOOK_GRAPH_DELEGATE_HPP

#include <ee/cocos/CocosFwd.hpp>

#ifdef EE_X_COCOS_JS
#include "ee/facebook/FacebookFwd.hpp"

namespace ee {
namespace facebook {
bool registerJsbIGraphDelegate(se::Object* global);
} // namespace facebook
} // namespace ee

#endif // EE_X_COCOS_JS

#endif /* EE_X_JSB_I_FACEBOOK_GRAPH_DELEGATE_HPP */