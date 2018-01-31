//
//  Vungle.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "Vungle.hpp"
#include "FunctionLogger.hpp"
#include "Utils.hpp"

#include <ee/Core.hpp>

#include <platform/CCPlatformConfig.h>

namespace eetest {
ee::Vungle* getVungle() {
    static auto plugin = ee::Vungle();
    static bool initialized;
    if (not initialized) {
        // Initialize Vungle on the main thread.
        ee::runOnUiThreadAndWait([] {
            FunctionLogger logger("Initialize Vungle");
            plugin.initialize(getVungleGameId(), "");
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

std::string getVungleRewardedVideoId() {
    return "rewarded";
}

void testVungleRewardedVideo() {
    // Create a Vungle rewarded video on the main thread.
    auto rewardedVideo = ee::runOnUiThreadAndWaitResult<
        std::shared_ptr<ee::RewardedVideoInterface>>([] {
        FunctionLogger logger("Create Vungle rewarded video");
        return getVungle()->createRewardedVideo(getVungleRewardedVideoId());
    });

    // Set the result callback.
    rewardedVideo->setResultCallback([](bool result) {
        FunctionLogger logger("Vungle rewarded video callback");
        logCurrentThread();
        getLogger().info("Result = ", result ? "succeeded" : "failed");
    });

    float delay = 0.0f;
    scheduleForever(delay += 5.0f, 5.0f, [rewardedVideo] {
        FunctionLogger logger("Show Vungle rewarded video");
        rewardedVideo->show();
    });
}
} // namespace eetest
