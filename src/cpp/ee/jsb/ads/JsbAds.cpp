//
//  jsb_ads.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/JsbAds.hpp"

#include "ee/jsb/ads/JsbIAdView.hpp"
#include "ee/jsb/ads/JsbIInterstitialAd.hpp"
#include "ee/jsb/ads/JsbIRewardedVideo.hpp"
#include "ee/jsb/ads/JsbMultiAdView.hpp"
#include "ee/jsb/ads/JsbMultiInterstitialAd.hpp"
#include "ee/jsb/ads/JsbMultiRewardedVideo.hpp"

namespace ee {
namespace ads {
bool registerJsb(se::Object* global) {
    registerJsbIAdView(global);
    registerJsbIInterstitialAd(global);
    registerJsbIRewardedVideo(global);
    registerJsbMultiAdView(global);
    registerJsbMultiInterstitialAd(global);
    registerJsbMultiRewardedVideo(global);
    return true;
}
} // namespace ads
} // namespace ee
