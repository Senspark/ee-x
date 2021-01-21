//
//  jsb_facebook.cpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#include "ee/facebook/JsbFacebook.hpp"

#include "ee/facebook/private/JsbFacebookBridge.hpp"
#include "ee/facebook/private/JsbFacebookGraphRequest.hpp"
#include "ee/facebook/private/JsbFacebookRequestContent.hpp"
#include "ee/facebook/private/JsbIFacebookAccessToken.hpp"
#include "ee/facebook/private/JsbIFacebookBridge.hpp"
#include "ee/facebook/private/JsbIFacebookGraphDelegate.hpp"
#include "ee/facebook/private/JsbIFacebookLoginDelegate.hpp"
#include "ee/facebook/private/JsbIFacebookRequestDelegate.hpp"
#include "ee/facebook/private/JsbIFacebookShareDelegate.hpp"

namespace ee {
namespace facebook {
bool registerJsb(se::Object* global) {
    registerJsbIAccessToken(global);
    registerJsbIBridge(global);
    registerJsbIGraphDelegate(global);
    registerJsbILoginDelegate(global);
    registerJsbIRequestDelegate(global);
    registerJsbIShareDelegate(global);
    registerJsbBridge(global);
    registerJsbGraphRequest(global);
    registerJsbRequestContent(global);
    return true;
}
} // namespace facebook
} // namespace ee
