//
//  Vungle.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "VungleTester.hpp"

#include <ee/Cpp.hpp>

#include "FunctionLogger.hpp"
#include "RewardedAdTester.hpp"
#include "Utils.hpp"

namespace eetest {
namespace vungle {
using Self = Tester;

void Self::initialize() {
    plugin_ = ee::PluginManager::createPlugin<ee::IVungle>();
    ee::noAwait([this]() -> ee::Task<> { //
        auto result = co_await plugin_->initialize(config::app_id);
    });
}

void Self::destroy() {
    plugin_->destroy();
}

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
} // namespace vungle
} // namespace eetest
