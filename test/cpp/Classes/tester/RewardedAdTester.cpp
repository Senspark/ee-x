//
//  RewardedAdTester.cpp
//  ee_x_test
//
//  Created by eps on 7/6/20.
//

#include "RewardedAdTester.hpp"

#include <ee/Cpp.hpp>

#include "Utils.hpp"

namespace eetest {
using Self = RewardedAdTester;

Self::RewardedAdTester(const std::shared_ptr<ee::IRewardedAd>& ad)
    : ad_(ad) {}

void Self::initialize() {}

void Self::destroy() {}

void Self::start() {
    scheduleForever(1.0f, 3.0f, [this] {
        getLogger().info("Load rewarded ad");
        ee::noAwait(ad_->load());
    });

    scheduleForever(2.0f, 3.0f, [this] {
        getLogger().info("Show rewarded ad");
        ee::noAwait(ad_->show());
    });
}

void Self::stop() {}
} // namespace eetest
