//
//  UnityAds.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "UnityAds.hpp"

#include <platform/CCPlatformConfig.h>

#include "FunctionLogger.hpp"
#include "Utils.hpp"

namespace eetest {
ee::UnityAds* getUnityAds() {
    static auto plugin = ee::UnityAds();
    static bool initialized;
    if (not initialized) {
        // Initialize Unity Ads on the main thread.
        FunctionLogger logger("Initialize Unity Ads");
        plugin.initialize(getUnityGameId(), false);
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

std::string getUnityRewardedAdId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "rewardedVideoZone";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "rewardedVideo";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

void testUnityAdsRewardedAd() {
    auto rewardedAd = getUnityAds()->createRewardedAd(getUnityRewardedAdId());
    auto interstitialAd =
        getUnityAds()->createInterstitialAd(getUnityInterstitialAdId());

    float delay = 0.0f;
    scheduleOnce(delay += 5.0f, ee::makeAwaiter([rewardedAd]() -> ee::Task<> {
                     FunctionLogger logger("Show UnityAds rewarded ad");
                     auto result = co_await rewardedAd->show();
                     logCurrentThread();
                     getLogger().info("Result = %d", static_cast<int>(result));
                 }));

    scheduleOnce(delay += 2.0f,
                 ee::makeAwaiter([interstitialAd]() -> ee::Task<> {
                     FunctionLogger logger("Show UnityAds interstitial ad");
                     co_await interstitialAd->show();
                     logCurrentThread();
                 }));
} // namespace eetest
} // namespace eetest
