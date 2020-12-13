//
//  AdMobFwd.hpp
//  ee_x
//
//  Created by Zinge on 10/16/17.
//
//

#ifndef EE_X_AD_MOB_FWD_HPP
#define EE_X_AD_MOB_FWD_HPP

#ifdef __cplusplus

#include <ee/ads/AdsFwd.hpp>

namespace ee {
namespace admob {
enum class BannerAdSize;
enum class AppOpenAdOrientation;

class IBridge;
class Bridge;
class BannerAd;
class NativeAd;
class NativeAdLayout;
class InterstitialAd;
class RewardedAd;
class AppOpenAd;
} // namespace admob

using IAdMob = admob::IBridge;
using AdMobAppOpenAdOrientation = admob::AppOpenAdOrientation;
using AdMobBannerAdSize = admob::BannerAdSize;
using AdMobNativeAdLayout = admob::NativeAdLayout;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_AD_MOB_FWD_HPP */
