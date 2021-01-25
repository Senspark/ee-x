//
//  jsb_facebook_graph_request.hpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#ifndef EE_X_JSB_FACEBOOK_GRAPH_REQUEST_HPP
#define EE_X_JSB_FACEBOOK_GRAPH_REQUEST_HPP

#include <ee/cocos/CocosFwd.hpp>

#ifdef EE_X_COCOS_JS
#include "ee/facebook/FacebookFwd.hpp"

namespace ee {
namespace facebook {
bool registerJsbGraphRequest(se::Object* global);
} // namespace facebook
} // namespace ee

#endif // EE_X_COCOS_JS

#endif /* EE_X_JSB_FACEBOOK_GRAPH_REQUEST_HPP */
