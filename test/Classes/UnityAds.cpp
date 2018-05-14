//
//  UnityAds.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "UnityAds.hpp"
#include "FunctionLogger.hpp"
#include "Utils.hpp"

#include <ee/Core.hpp>

#include <platform/CCPlatformConfig.h>

namespace eetest {
ee::UnityAds* getUnityAds() {
    static auto plugin = ee::UnityAds();
    static bool initialized;
    if (not initialized) {
        // Initialize Unity Ads on the main thread.
        ee::runOnUiThreadAndWait([] {
            FunctionLogger logger("Initialize Unity Ads");
            plugin.initialize(getUnityGameId(), false);
        });
        initialized = true;
    }
    return &plugin;
}

std::string getUnityGameId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "73406";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "1423604";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
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
        ee::runOnUiThreadAndWaitResult<std::shared_ptr<ee::IRewardedVideo>>([] {
            FunctionLogger logger("Create Unity Ads rewarded video");
            return getUnityAds()->createRewardedVideo(
                getUnityRewardedVideoId());
        });

    rewardedVideo->setResultCallback([](bool result) {
        FunctionLogger logger("Unity Ads rewarded video callback");
        logCurrentThread();
        getLogger().info("Result = ", result ? "succeeded" : "failed");
    });

    auto interstitialAd =
        ee::runOnUiThreadAndWaitResult<std::shared_ptr<ee::IInterstitialAd>>(
            [] {
                FunctionLogger logger("Create Unity Ads interstitial ad");
                return getUnityAds()->createInterstitialAd(
                    getUnityInterstitialAdId());
            });

    interstitialAd->setResultCallback([] {
        FunctionLogger logger("Unity Ads interstitial ad callback");
        logCurrentThread();
    });

    float delay = 0.0f;
    scheduleOnce(delay += 5.0f, [rewardedVideo] {
        FunctionLogger logger("Show UnityAds rewarded video");
        rewardedVideo->show();
    });

    scheduleOnce(delay += 2.0f, [interstitialAd] {
        FunctionLogger logger("Show UnityAds interstitial ad");
        interstitialAd->show();
    });
}
} // namespace eetest
