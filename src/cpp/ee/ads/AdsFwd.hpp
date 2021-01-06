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

class IAdView;
class IFullScreenAd;

class MultiAdView;
class MultiFullScreenAd;

class NullAdView;
class NullFullScreenAd;

class GuardedAdView;
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

using ads::IAdView;
using ads::IFullScreenAd;
using IInterstitialAd [[deprecated]] = IFullScreenAd;
using IRewardedAd [[deprecated]] = IFullScreenAd;

using ads::MultiAdView;
using ads::MultiFullScreenAd;
using MultiInterstitialAd [[deprecated]] = MultiFullScreenAd;
using MultiRewardedAd [[deprecated]] = MultiFullScreenAd;

using ads::NullAdView;
using ads::NullFullScreenAd;
using NullInterstitialAd [[deprecated]] = NullFullScreenAd;
using NullRewardedAd [[deprecated]] = NullFullScreenAd;

using IRewardedVideo [[deprecated]] = IRewardedAd;
using MultiRewardedVideo [[deprecated]] = MultiRewardedAd;
using NullRewardedVideo [[deprecated]] = NullRewardedAd;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_ADS_FWD_HPP */
