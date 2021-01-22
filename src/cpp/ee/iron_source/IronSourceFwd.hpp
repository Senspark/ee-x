#ifndef EE_X_IRON_SOURCE_FWD_HPP
#define EE_X_IRON_SOURCE_FWD_HPP

#ifdef __cplusplus

#include <ee/ads/AdsFwd.hpp>

namespace ee {
namespace iron_source {
enum class BannerAdSize;
class IBridge;
class Bridge;
class BannerAd;
class InterstitialAd;
class RewardedAd;
} // namespace iron_source

using IronSourceBannerAdSize = iron_source::BannerAdSize;
using IIronSource = iron_source::IBridge;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_IRON_SOURCE_FWD_HPP */
