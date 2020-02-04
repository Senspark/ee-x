//
//  jsb_facebook.cpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#include "ee/jsb/JsbFacebook.hpp"

#include "ee/jsb/facebook/JsbFacebookBridge.hpp"
#include "ee/jsb/facebook/JsbFacebookGraphRequest.hpp"
#include "ee/jsb/facebook/JsbFacebookRequestContent.hpp"
#include "ee/jsb/facebook/JsbIFacebookAccessToken.hpp"
#include "ee/jsb/facebook/JsbIFacebookBridge.hpp"
#include "ee/jsb/facebook/JsbIFacebookGraphDelegate.hpp"
#include "ee/jsb/facebook/JsbIFacebookLoginDelegate.hpp"
#include "ee/jsb/facebook/JsbIFacebookRequestDelegate.hpp"
#include "ee/jsb/facebook/JsbIFacebookShareDelegate.hpp"

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
