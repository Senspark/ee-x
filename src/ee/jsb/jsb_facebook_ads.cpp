//
//  jsb_facebook_ads.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/jsb_facebook_ads.hpp"

#include "ee/jsb/facebookads/jsb_facebook_ads_bridge.hpp"
#include "ee/jsb/facebookads/jsb_facebook_native_ads_layout.hpp"

namespace ee {
namespace facebook {
bool register_all_facebook_ads_manual(se::Object* object) {
    register_facebookads_bridge_manual(object);
    register_facebook_native_ad_layout_manual(object);
    return true;
}
} // namespace facebook
} // namespace ee
