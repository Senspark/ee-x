//
//  IronSource.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "IronSource.hpp"

#include <platform/CCPlatformConfig.h>

#include "FunctionLogger.hpp"
#include "Utils.hpp"

namespace eetest {
ee::IronSource* getIronSource() {
    static auto plugin = std::make_unique<ee::IronSource>();
    static bool initialized;
    if (not initialized) {
        FunctionLogger logger("Initialize ironSource");
        plugin->initialize(getIronSourceGameId());
        initialized = true;
    }
    return plugin.get();
}

std::string getIronSourceGameId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "67a6443d";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "67a60ab5";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

std::string getIronSourceRewardedAdId() {
    return "DefaultRewardedVideo";
}

void testIronSourceRewardedAd() {
    auto ad = std::make_shared<ee::GuardedRewardedAd>(
        getIronSource()->createRewardedAd(getIronSourceRewardedAdId()));

    float delay = 0.0f;
    scheduleForever(delay += 5.0f, 5.0f, ee::makeAwaiter([ad]() -> ee::Task<> {
                        FunctionLogger logger("Show ironSource rewarded ad");
                        co_await ad->show();
                        logCurrentThread();
                    }));
}
} // namespace eetest
