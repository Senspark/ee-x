
//
//  MediationManager.cpp
//  ee_x
//
//  Created by Zinge on 10/23/17.
//
//

#include "ee/ads/internal/MediationManager.hpp"

#include "ee/ads/internal/AsyncHelper.hpp"

namespace ee {
namespace ads {
using Self = MediationManager;

Self& Self::getInstance() {
    static Self sharedInstance;
    return sharedInstance;
}

Self::MediationManager() {
    interstitialAdDisplayer_ = std::make_shared<AsyncHelper<bool>>();
    rewardedAdDisplayer_ = std::make_shared<AsyncHelper<IRewardedAdResult>>();
}

Self::~MediationManager() {}

const std::shared_ptr<IAsyncHelper<bool>>&
Self::getInterstitialAdDisplayer() const {
    return interstitialAdDisplayer_;
}

const std::shared_ptr<IAsyncHelper<IRewardedAdResult>>&
Self::getRewardedAdDisplayer() const {
    return rewardedAdDisplayer_;
}
} // namespace ads
} // namespace ee
