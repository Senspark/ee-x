//
//  jsb_facebook_ads.h
//  ee-x
//
//  Created by Le Van Kiet on 8/21/18.
//

#ifndef jsb_facebook_ads_h
#define jsb_facebook_ads_h

#ifdef __cplusplus
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

#endif
#endif /* jsb_facebook_ads_h */
