//
//  IronSource.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "IronSourceTester.hpp"

#include <ee/IronSource.hpp>

#include "FunctionLogger.hpp"
#include "RewardedAdTester.hpp"

namespace eetest {
namespace iron_source {
using Self = Tester;

void Self::initialize() {
    FunctionLogger logger(__PRETTY_FUNCTION__);
    plugin_ = std::make_shared<ee::IronSource>();
}

void Self::destroy() {}

void Self::start() {
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
} // namespace iron_source
} // namespace eetest
