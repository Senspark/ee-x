//
//  AdMobFwd.hpp
//  ee_x
//
//  Created by Zinge on 10/16/17.
//
//

#ifndef EE_X_ADMOB_FWD_HPP
#define EE_X_ADMOB_FWD_HPP

#ifdef __cplusplus

#include <ee/AdsFwd.hpp>

namespace ee {
namespace admob {
enum class BannerAdSize;

class IBridge;
class Bridge;
class BannerAd;
class NativeAd;
class NativeAdLayout;
class InterstitialAd;
class RewardedAd;
} // namespace admob

using IAdMob = admob::IBridge;
using AdMobBannerAdSize = admob::BannerAdSize;
using AdMobNativeAdLayout = admob::NativeAdLayout;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_ADMOB_FWD_HPP */
