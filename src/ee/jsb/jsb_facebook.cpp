//
//  jsb_facebook.cpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#include "ee/jsb/jsb_facebook.hpp"

#include "facebook/jsb_cc_facebook_bridge_impl.hpp"
//#include "facebook/jsb_cc_facebook_graph_request.hpp"
//#include "facebook/jsb_cc_facebook_request_content.hpp"
#include "facebook/jsb_cc_ifacebook_access_token.hpp"
#include "facebook/jsb_cc_ifacebook_bridge.hpp"
#include "facebook/jsb_cc_ifacebook_graph_delegate.hpp"
#include "facebook/jsb_cc_ifacebook_login_delegate.hpp"
#include "facebook/jsb_cc_ifacebook_request_delegate.hpp"
#include "facebook/jsb_cc_ifacebook_share_delegate.hpp"

namespace ee {
namespace facebook {
bool register_all_facebook_manual(se::Object* object) {
    register_ifacebook_bridge_manual(object);
    register_ifacebook_access_token_manual(object);
    register_ifacebook_graph_delegate_manual(object);
    register_ifacebook_login_delegate_manual(object);
    register_ifacebook_share_delegate_manual(object);
    register_ifacebook_request_delegate(object);
    register_facebook_bridge_impl_manual(object);
//    register_facebook_graph_request_manual(object);
//    register_facebook_request_content_manual(object);
    
    return true;
}
} // namespace facebook
} // namespace ee
