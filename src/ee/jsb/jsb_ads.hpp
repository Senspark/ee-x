//
//  jsb_ads.hpp
//  ee-x
//
//  Created by Duc Nguyen on 7/17/18.
//

#ifndef JSB_ADS_HPP__
#define JSB_ADS_HPP__

#ifdef __cplusplus
#include "ee/jsb/ads/jsb_adview.hpp"
#include "ee/jsb/ads/jsb_interstitial_ad.hpp"
#include "ee/jsb/ads/jsb_multi_adview.hpp"
#include "ee/jsb/ads/jsb_multi_interstitial_ad.hpp"
#include "ee/jsb/ads/jsb_multi_rewarded_video.hpp"
#include "ee/jsb/ads/jsb_rewarded_video.hpp"

namespace ee {
namespace ads {
bool register_all_ads_manual(se::Object* object) {
    register_adview_manual(object);
    register_interstitial_ad_manual(object);
    register_rewarded_video_manual(object);
    register_multi_interstitial_ad_manual(object);
    register_multi_rewarded_video_manual(object);
    register_multi_adview_manual(object);
    return true;
}
} // namespace ads
} // namespace ee
#endif // __cplusplus

#endif // JSB_CORE_HPP
