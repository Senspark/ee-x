//
//  AdsFwd.hpp
//  ee_x
//
//  Created by Zinge on 10/18/17.
//
//

#ifndef EE_X_ADS_FWD_HPP
#define EE_X_ADS_FWD_HPP

#include <ee/CoreFwd.hpp>

namespace ee {
namespace ads {
struct IAdViewObserver;

class IAdView;
class IInterstitialAd;
class IRewardedVideo;

class MultiAdView;
class MultiInterstitialAd;
class MultiRewardedVideo;

class NullAdView;
class NullInterstitialAd;
class NullRewardedVideo;

class MediationManager;
} // namespace ads

using ads::IAdViewObserver;

using ads::IAdView;
using ads::IInterstitialAd;
using ads::IRewardedVideo;

using ads::MultiAdView;
using ads::MultiInterstitialAd;
using ads::MultiRewardedVideo;

using ads::NullAdView;
using ads::NullInterstitialAd;
using ads::NullRewardedVideo;
} // namespace ee

#endif /* EE_X_ADS_FWD_HPP */
