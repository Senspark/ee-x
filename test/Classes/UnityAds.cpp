//
//  UnityAds.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "UnityAds.hpp"
#include "Utils.hpp"

#include <ee/Core.hpp>

#include <platform/CCPlatformConfig.h>

namespace eetest {
ee::UnityAds* getUnityAds() {
    static auto plugin = ee::UnityAds();
    static bool initialized;
    if (not initialized) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        constexpr auto gameId = "73406";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        constexpr auto gameId = "1423604";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        ee::runOnUiThreadAndWait([] { plugin.initialize(gameId, false); });
        initialized = true;
    }
    return &plugin;
}

std::string getUnityInterstitialAdId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "defaultZone";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "video";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

std::string getUnityRewardedVideoId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "rewardedVideoZone";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "rewardedVideo";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

void testUnityAdsRewardedVideo() {
    auto rewardedVideo =
        getUnityAds()->createRewardedVideo(getUnityRewardedVideoId());
    rewardedVideo->setResultCallback([](bool result) {
        logCurrentThread();
        getLogger().info("Result = ", result ? "succeeded" : "failed");
    });

    auto interstitialAd =
        getUnityAds()->createInterstitialAd(getUnityInterstitialAdId());
    interstitialAd->setResultCallback([] {
        logCurrentThread();
        getLogger().info("Done");
    });

    float delay = 0.0f;
    scheduleOnce(delay += 5.0f, [rewardedVideo] {
        getLogger().info("Show UnityAds rewarded video");
        rewardedVideo->show();
    });

    scheduleOnce(delay += 2.0f, [interstitialAd] {
        getLogger().info("Show UnityAds interstitial ad");
        interstitialAd->show();
    });
}
} // namespace eetest
