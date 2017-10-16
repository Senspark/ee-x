//
//  AdMobFwd.hpp
//  ee_x
//
//  Created by Zinge on 10/16/17.
//
//

#ifndef EE_X_ADMOB_FWD_HPP
#define EE_X_ADMOB_FWD_HPP

namespace ee {
namespace admob {
enum class BannerAdSize;

class AdMob;
class BannerAd;
class NativeAd;
class NativeAdLayout;
class InterstitialAd;
class RewardedVideo;
} // namespace admob

using admob::AdMob;
using AdMobBannerAdSize = admob::BannerAdSize;
using AdMobNativeAdLayout = admob::NativeAdLayout;
} // namespace ee

#endif /* EE_X_ADMOB_FWD_HPP */
