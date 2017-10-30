//
//  AdsFwd.hpp
//  ee_x
//
//  Created by Zinge on 10/18/17.
//
//

#ifndef EE_X_ADS_FWD_HPP
#define EE_X_ADS_FWD_HPP

namespace ee {
namespace ads {
class AdViewInterface;
class InterstitialAdInterface;
class RewardedVideoInterface;

class MultiAdView;
class MultiInterstitialAd;
class MultiRewardedVideo;

class NullAdView;
class NullInterstitialAd;
class NullRewardedVideo;

class MediationManager;
} // namespace ads

using ads::AdViewInterface;
using ads::InterstitialAdInterface;
using ads::RewardedVideoInterface;

using ads::MultiAdView;
using ads::MultiInterstitialAd;
using ads::MultiRewardedVideo;

using ads::NullAdView;
using ads::NullInterstitialAd;
using ads::NullRewardedVideo;
} // namespace ee

#endif /* EE_X_ADS_FWD_HPP */
