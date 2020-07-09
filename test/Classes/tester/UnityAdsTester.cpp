//
//  UnityAds.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "UnityAdsTester.hpp"

#include <ee/UnityAds.hpp>

#include "FunctionLogger.hpp"
#include "InterstitialAdTester.hpp"
#include "RewardedAdTester.hpp"
#include "Utils.hpp"

namespace eetest {
namespace unity_ads {
using Self = Tester;

void Self::initialize() {
    FunctionLogger logger(__PRETTY_FUNCTION__);
    plugin_ = std::make_shared<ee::UnityAds>();
    plugin_->initialize(config::app_id, true);
}

void Self::destroy() {
    plugin_->destroy();
}

void Self::start() {
    testers_.push_back(std::make_shared<InterstitialAdTester>(
        plugin_->createInterstitialAd(config::interstitial_ad_id)));
    testers_.push_back(std::make_shared<RewardedAdTester>(
        plugin_->createRewardedAd(config::rewarded_ad_id)));

    for (auto&& tester : testers_) {
        tester->initialize();
        tester->start();
    }
}

void Self::stop() {
    for (auto&& tester : testers_) {
        tester->stop();
        tester->destroy();
    }
    testers_.clear();
}
} // namespace unity_ads
} // namespace eetest
