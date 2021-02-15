//
//  AdsFwd.hpp
//  ee_x
//
//  Created by Zinge on 10/18/17.
//
//

#ifndef EE_X_ADS_FWD_HPP
#define EE_X_ADS_FWD_HPP

#ifdef __cplusplus

#include <ee/core/CoreFwd.hpp>

namespace ee {
namespace ads {
enum class FullScreenAdResult;

struct AdObserver;

class IAd;
class IBannerAd;
class IFullScreenAd;

class MultiBannerAd;
class MultiFullScreenAd;

class NullAd;
class NullBannerAd;
class NullFullScreenAd;

class GuardedBannerAd;
class GuardedFullScreenAd;

class MessageHelper;

template <class Result>
class IAsyncHelper;

template <class Result>
class AsyncHelper;

class Retrier;
class MediationManager;
} // namespace ads

using ads::FullScreenAdResult;
using IRewardedAdResult [[deprecated]] = FullScreenAdResult;

using ads::AdObserver;
using IAdViewObserver [[deprecated]] = AdObserver;
using IInterstitialAdObserver [[deprecated]] = AdObserver;
using IRewardedAdObserver [[deprecated]] = AdObserver;

using ads::IAd;
using ads::IBannerAd;
using ads::IFullScreenAd;
using IAdView [[deprecated]] = IBannerAd;
using IInterstitialAd [[deprecated]] = IFullScreenAd;
using IRewardedAd [[deprecated]] = IFullScreenAd;

using ads::MultiBannerAd;
using ads::MultiFullScreenAd;
using MultiAdView [[deprecated]] = MultiBannerAd;
using MultiInterstitialAd [[deprecated]] = MultiFullScreenAd;
using MultiRewardedAd [[deprecated]] = MultiFullScreenAd;

using ads::NullAd;
using ads::NullBannerAd;
using ads::NullFullScreenAd;
using NullAdView [[deprecated]] = NullBannerAd;
using NullInterstitialAd [[deprecated]] = NullFullScreenAd;
using NullRewardedAd [[deprecated]] = NullFullScreenAd;

using IRewardedVideo [[deprecated]] = IFullScreenAd;
using MultiRewardedVideo [[deprecated]] = MultiFullScreenAd;
using NullRewardedVideo [[deprecated]] = NullFullScreenAd;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_ADS_FWD_HPP */
