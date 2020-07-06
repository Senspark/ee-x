//
//  AppLovin.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "AppLovinTester.hpp"

#include <ee/AppLovin.hpp>

#include "RewardedAdTester.hpp"

namespace eetest {
namespace app_lovin {
using Self = Tester;

void Self::initialize() {
    plugin_ = std::make_shared<ee::AppLovin>();
    plugin_->initialize(config::app_id);
    plugin_->setMuted(false);
    plugin_->setVerboseLogging(true);
}

void Self::destroy() {
    plugin_->destroy();
}

void Self::start() {
    testers_.push_back(
        std::make_shared<RewardedAdTester>(plugin_->createRewardedAd()));

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
} // namespace app_lovin
} // namespace eetest
