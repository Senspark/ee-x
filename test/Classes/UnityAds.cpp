//
//  UnityAds.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "UnityAds.hpp"

#include <platform/CCPlatformConfig.h>

#include <ee/Coroutine.hpp>

#include "FunctionLogger.hpp"
#include "Utils.hpp"

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

std::string getUnityRewardedAdId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "rewardedVideoZone";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "rewardedVideo";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

void testUnityAdsRewardedAd() {
    auto rewardedAd =
        ee::runOnUiThreadAndWaitResult<std::shared_ptr<ee::IRewardedAd>>([] {
            FunctionLogger logger("Create Unity Ads rewarded ad");
            return std::make_shared<ee::GuardedRewardedAd>(
                getUnityAds()->createRewardedAd(getUnityRewardedAdId()));
        });

    auto interstitialAd =
        ee::runOnUiThreadAndWaitResult<std::shared_ptr<ee::IInterstitialAd>>(
            [] {
                FunctionLogger logger("Create Unity Ads interstitial ad");
                return getUnityAds()->createInterstitialAd(
                    getUnityInterstitialAdId());
            });

    float delay = 0.0f;
    scheduleOnce(delay += 5.0f, [rewardedAd] {
        ee::runOnUiThread(ee::makeAwaiter([rewardedAd]() -> ee::Task<> {
            FunctionLogger logger("Show UnityAds rewarded ad");
            auto result = co_await rewardedAd->show();
            logCurrentThread();
            getLogger().info("Result = %d", static_cast<int>(result));
        }));
    });

    scheduleOnce(delay += 2.0f, [interstitialAd] {
        ee::runOnUiThread(ee::makeAwaiter([interstitialAd]() -> ee::Task<> {
            FunctionLogger logger("Show UnityAds interstitial ad");
            co_await interstitialAd->show();
            logCurrentThread();
        }));
    });
}
} // namespace eetest
