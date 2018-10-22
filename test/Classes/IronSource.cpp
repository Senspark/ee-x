//
//  IronSource.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "IronSource.hpp"
#include "FunctionLogger.hpp"
#include "Utils.hpp"

#include <ee/Core.hpp>

#include <platform/CCPlatformConfig.h>

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

std::string getIronSourceRewardedVideoId() {
    return "DefaultRewardedVideo";
}

void testIronSourceRewardedVideo() {
    auto rewardedVideo =
        ee::runOnUiThreadAndWaitResult<std::shared_ptr<ee::IRewardedVideo>>([] {
            FunctionLogger logger("Create ironSource rewarded video");
            return getIronSource()->createRewardedVideo(
                getIronSourceRewardedVideoId());
        });

    float delay = 0.0f;
    scheduleForever(delay += 5.0f, 5.0f, [rewardedVideo] {
        ee::runOnUiThread([rewardedVideo] {
            FunctionLogger logger("Show ironSource rewarded video");
            rewardedVideo->show();
        });
    });
}
} // namespace eetest
