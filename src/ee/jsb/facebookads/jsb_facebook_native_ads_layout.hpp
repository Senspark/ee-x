//
//  jsb_facebook_native_ads_layout_hpp
//  ee-x
//
//  Created by Le Van Kiet on 8/21/18.
//

#ifndef jsb_facebook_native_ads_layout_hpp
#define jsb_facebook_native_ads_layout_hpp

#include "ee/jsb/core/jsb_templates.hpp"
#include "ee/jsb/jsb_facebook_ads_fwd.hpp"
#include "ee/jsb/jsb_fwd.hpp"

namespace ee {
namespace core {
template <>
ee::facebook::NativeAdLayout
get_value<ee::facebook::NativeAdLayout>(const se::Value& value);
} // namespace core

namespace facebook {
bool register_facebook_native_ad_layout_manual(se::Object* object);
} // namespace facebook
} // namespace ee

#endif /* jsb_facebook_native_ads_layout_hpp */
