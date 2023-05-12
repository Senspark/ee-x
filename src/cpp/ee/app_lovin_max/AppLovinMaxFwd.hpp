#ifndef EE_X_APP_LOVIN_MAX_FWD_HPP
#define EE_X_APP_LOVIN_MAX_FWD_HPP

#ifdef __cplusplus

#include <ee/ads/AdsFwd.hpp>

namespace ee {
namespace app_lovin_max {
enum class BannerAdSize;
class IBridge;
class Bridge;
class InterstitialAd;
class RewardedAd;
} // namespace app_lovin_max
using MaxBannerAdSize = app_lovin_max::BannerAdSize;
using IAppLovinMax = app_lovin_max::IBridge;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_APP_LOVIN_MAX_FWD_HPP */
