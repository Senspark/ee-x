//
//  InterstitialAdTester.cpp
//  ee_x_test
//
//  Created by eps on 7/6/20.
//

#include "InterstitialAdTester.hpp"

#include <ee/Cpp.hpp>

#include "Utils.hpp"

namespace eetest {
using Self = InterstitialAdTester;

Self::InterstitialAdTester(const std::shared_ptr<ee::IInterstitialAd>& ad)
    : ad_(ad) {}

void Self::initialize() {}

void Self::destroy() {}

void Self::start() {
    scheduleForever(1.0f, 3.0f, [this] {
        getLogger().info("Load interstitial ad");
        ee::noAwait(ad_->load());
    });

    scheduleForever(2.0f, 3.0f, [this] {
        getLogger().info("Show interstitial ad");
        ee::noAwait(ad_->show());
    });
}

void Self::stop() {}
} // namespace eetest
