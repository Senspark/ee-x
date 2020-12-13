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
enum class IRewardedAdResult;

struct IAdViewObserver;
struct IInterstitialAdObserver;
struct IRewardedAdObserver;

class IAdView;
class IInterstitialAd;
class IRewardedAd;

class MultiAdView;
class MultiInterstitialAd;
class MultiRewardedAd;

class NullAdView;
class NullInterstitialAd;
class NullRewardedAd;

class GuardedAdView;
class GuardedInterstitialAd;
class GuardedRewardedAd;

class MessageHelper;

template <class Result>
class IAsyncHelper;

template <class Result>
class AsyncHelper;

class MediationManager;
} // namespace ads

using ads::IRewardedAdResult;

using ads::IAdViewObserver;
using ads::IInterstitialAdObserver;
using ads::IRewardedAdObserver;

using ads::IAdView;
using ads::IInterstitialAd;
using ads::IRewardedAd;

using ads::MultiAdView;
using ads::MultiInterstitialAd;
using ads::MultiRewardedAd;

using ads::NullAdView;
using ads::NullInterstitialAd;
using ads::NullRewardedAd;

using IRewardedVideo [[deprecated]] = IRewardedAd;
using MultiRewardedVideo [[deprecated]] = MultiRewardedAd;
using NullRewardedVideo [[deprecated]] = NullRewardedAd;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_ADS_FWD_HPP */
