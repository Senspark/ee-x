//
//  IronSource.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "IronSource.hpp"

#include <platform/CCPlatformConfig.h>

#include <ee/Coroutine.hpp>

#include "FunctionLogger.hpp"
#include "Utils.hpp"

namespace eetest {
ee::IronSource* getIronSource() {
    static auto plugin = std::make_unique<ee::IronSource>();
    static bool initialized;
    if (not initialized) {
        ee::runOnUiThreadAndWait([] {
            FunctionLogger logger("Initialize ironSource");
            plugin->initialize(getIronSourceGameId());
        });
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
    auto ad =
        ee::runOnUiThreadAndWaitResult<std::shared_ptr<ee::IRewardedAd>>([] {
            FunctionLogger logger("Create ironSource rewarded ad");
            return getIronSource()->createRewardedAd(
                getIronSourceRewardedAdId());
        });

    float delay = 0.0f;
    scheduleForever(delay += 5.0f, 5.0f, [ad] {
        ee::runOnUiThread(ee::makeAwaiter([ad]() -> ee::Task<> {
            FunctionLogger logger("Show ironSource rewarded ad");
            co_await ad->show();
        }));
    });
}
} // namespace eetest
