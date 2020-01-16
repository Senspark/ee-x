//
//  FacebookAds.h
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_FACEBOOK_ADS_FWD_HPP_
#define EE_X_FACEBOOK_ADS_FWD_HPP_

#include <ee/AdsFwd.hpp>

namespace ee {
namespace facebook {
enum class BannerAdSize;

class FacebookAds;
class BannerAd;
class NativeAd;
class NativeAdLayout;
class InterstitialAd;
class RewardedVideo;
} // namespace facebook

using facebook::FacebookAds;
using FacebookBannerAdSize = facebook::BannerAdSize;
using FacebookNativeAdLayout = facebook::NativeAdLayout;
} // namespace ee

#endif /* EE_X_FACEBOOK_ADS_FWD_HPP_ */
