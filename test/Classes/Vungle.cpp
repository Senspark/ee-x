//
//  Vungle.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "Vungle.hpp"
#include "Utils.hpp"

#include <ee/Core.hpp>

#include <platform/CCPlatformConfig.h>

namespace eetest {
ee::Vungle* getVungle() {
    static auto plugin = ee::Vungle();
    static bool initialized;
    if (not initialized) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        constexpr auto gameId = "com.senspark.goldminerclassic";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        constexpr auto gameId = "651916412";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        ee::runOnUiThreadAndWait([] { plugin.initialize(gameId); });
        initialized = true;
    }
    return &plugin;
}

std::string getVungleRewardedVideoId() {
    return "rewarded";
}

void testVungleRewardedVideo() {
    auto rewardedVideo = getVungle()->createRewardedVideo();
    rewardedVideo->setResultCallback([](bool result) {
        logCurrentThread();
        getLogger().info("Result = ", result ? "succeeded" : "failed");
    });

    float delay = 0.0f;
    scheduleForever(delay += 5.0f, 5.0f, [rewardedVideo] {
        getLogger().info("Show Vungle rewarded video");
        rewardedVideo->show();
    });
}
} // namespace eetest
