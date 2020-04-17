//
//  Vungle.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "Vungle.hpp"

#include <platform/CCPlatformConfig.h>

#include <ee/Coroutine.hpp>

#include "FunctionLogger.hpp"
#include "Utils.hpp"

namespace eetest {
ee::Vungle* getVungle() {
    static auto plugin = ee::Vungle();
    static bool initialized;
    if (not initialized) {
        // Initialize Vungle on the main thread.
        ee::runOnUiThreadAndWait([] {
            FunctionLogger logger("Initialize Vungle");
            plugin.initialize(getVungleGameId());
        });
        initialized = true;
    }
    return &plugin;
}

std::string getVungleGameId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "com.senspark.goldminerclassic";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "651916412";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

std::string getVungleRewardedAdId() {
    return "rewarded";
}

void testVungleRewardedAd() {
    // Create a Vungle rewarded video on the main thread.
    auto ad =
        ee::runOnUiThreadAndWaitResult<std::shared_ptr<ee::IRewardedAd>>([] {
            FunctionLogger logger("Create Vungle rewarded ad");
            return getVungle()->createRewardedAd(getVungleRewardedAdId());
        });

    float delay = 0.0f;
    scheduleForever(delay += 5.0f, 5.0f, [ad] {
        ee::runOnUiThread(ee::makeAwaiter([ad]() -> ee::Task<> {
            FunctionLogger logger("Show Vungle rewarded ad");
            auto result = co_await ad->show();
            logCurrentThread();
            getLogger().info("Result = %d", static_cast<int>(result));
        }));
    });
}
} // namespace eetest
