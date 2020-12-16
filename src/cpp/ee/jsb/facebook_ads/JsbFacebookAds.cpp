//
//  jsb_facebook_ads.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/JsbFacebookAds.hpp"

#include "ee/jsb/facebook_ads/JsbFacebookAdsBridge.hpp"
#include "ee/jsb/facebook_ads/JsbFacebookNativeAdLayout.hpp"

namespace ee {
namespace facebook_ads {
bool registerJsb(se::Object* global) {
    registerJsbBridge(global);
    registerJsbNativeAdLayout(global);
    return true;
}
} // namespace facebook_ads
} // namespace ee
