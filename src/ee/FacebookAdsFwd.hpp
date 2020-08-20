//
//  FacebookAds.h
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_FACEBOOK_ADS_FWD_HPP
#define EE_X_FACEBOOK_ADS_FWD_HPP

#ifdef __cplusplus

#include <ee/AdsFwd.hpp>

namespace ee {
namespace facebook_ads {
enum class BannerAdSize;

class IBridge;
class Bridge;
class BannerAd;
class NativeAd;
class NativeAdLayout;
class InterstitialAd;
class RewardedAd;
} // namespace facebook_ads

using IFacebookAds = facebook_ads::IBridge;
using FacebookBannerAdSize = facebook_ads::BannerAdSize;
using FacebookNativeAdLayout = facebook_ads::NativeAdLayout;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_FACEBOOK_ADS_FWD_HPP */
