//
//  IronSource.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "IronSource.hpp"
#include "Utils.hpp"

#include <ee/Core.hpp>

#include <platform/CCPlatformConfig.h>

namespace eetest {
ee::IronSource* getIronSource() {
    static auto plugin = ee::IronSource();
    static bool initialized;
    if (not initialized) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        constexpr auto gameId = "67a6443d";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        constexpr auto gameId = "67a60ab5";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        ee::runOnUiThreadAndWait([] {
            getLogger().info("Initialize ironSource begin");
            logCurrentThread();
            plugin.initialize(gameId);
            getLogger().info("Initialize ironSource end");
        });
        initialized = true;
    }
    return &plugin;
}

std::string getIronSourceRewardedVideoId() {
    return "DefaultRewardedVideo";
}

void testIronSourceRewardedVideo() {
    getLogger().info("Create ironSource rewarded video begin");
    auto rewardedVideo =
        getIronSource()->createRewardedVideo(getIronSourceRewardedVideoId());
    getLogger().info("Create ironSource rewarded video end");

    float delay = 0.0f;
    scheduleForever(delay += 5.0f, 5.0f, [rewardedVideo] {
        getLogger().info("Show ironSource rewarded video begin");
        rewardedVideo->show();
        getLogger().info("Show ironSource rewarded video end");
    });
}
} // namespace eetest
