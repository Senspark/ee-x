//
//  UnityAds.h
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_UNITY_ADS_FWD_HPP
#define EE_X_UNITY_ADS_FWD_HPP

#ifdef __cplusplus

#include <ee/ads/AdsFwd.hpp>

namespace ee {
namespace unity_ads {
enum class BannerAdSize;
class IBridge;
class Bridge;
class InterstitialAd;
class RewardedAd;
} // namespace unity_ads

using UnityBannerAdSize = unity_ads::BannerAdSize;
using IUnityAds = unity_ads::IBridge;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_UNITY_ADS_FWD_HPP */
