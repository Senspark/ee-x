//
//  FacebookAds.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "FacebookAds.hpp"
#include "Utils.hpp"

#include <ee/Core.hpp>

#include <base/CCDirector.h>
#include <platform/CCGLView.h>

namespace eetest {
ee::FacebookAds* getFacebookAds() {
    static auto plugin = ee::FacebookAds();
    static bool initialized = false;
    if (not initialized) {
        ee::runOnUiThreadAndWait([] {
            plugin.clearTestDevices();
            plugin.addTestDevice(plugin.getTestDeviceHash());
            plugin.addTestDevice(
                "ad45c323f6a9b07f7a9c072549efb279"); // BlueStacks.
        });
        initialized = true;
    }
    return &plugin;
}

std::string getFacebookInterstitialAdId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "869337403086643_1447442535276124";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "869337403086643_1447441308609580";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

std::shared_ptr<ee::AdViewInterface> createFacebookNativeAd() {
    return getFacebookAds()->createNativeAd(
        ee::FacebookNativeAdBuilder()
            .setAdId("869337403086643_1444948412192203")
            .setLayoutName("fb_native_spin")
            .setIcon("native_ad_icon")
            .setTitle("native_ad_title")
            .setMedia("native_ad_media")
            .setSocialContext("native_ad_social_context")
            .setAdChoices("ad_choices_container")
            .setBody("native_ad_body")
            .setAction("native_ad_call_to_action"));
}

std::shared_ptr<ee::InterstitialAdInterface> createFacebookInterstitialAd() {
    return getFacebookAds()->createInterstitialAd(
        getFacebookInterstitialAdId());
}

void testFacebookNativeAd() {
    auto&& frameSize =
        cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
    int screenWidth = static_cast<int>(frameSize.width);
    int screenHeight = static_cast<int>(frameSize.height);

    std::shared_ptr<ee::AdViewInterface> nativeAd;
    ee::runOnUiThreadAndWait([&nativeAd] {
        getLogger().info("Create Facebook native ad");
        nativeAd = createFacebookNativeAd();
        nativeAd->setVisible(true);
    });

    float delay = 0.0f;
    scheduleForever(delay += 1.0f, 5.0f, [nativeAd] {
        ee::runOnUiThread([nativeAd] { nativeAd->load(); });
    });

    scheduleForever(delay += 1.0f, 8.0f, [screenWidth, screenHeight, nativeAd] {
        ee::runOnUiThread([screenWidth, screenHeight, nativeAd] {
            getLogger().info("Resize = screen size / 4");
            nativeAd->setPosition(3 * screenWidth / 8, 3 * screenHeight / 8);
            nativeAd->setSize(screenWidth / 4, screenHeight / 4);
        });
    });

    scheduleForever(delay += 1.0f, 8.0f, [nativeAd] {
        ee::runOnUiThread([nativeAd] {
            getLogger().info("Move to top-left");
            nativeAd->setPosition(0, 0);
        });
    });

    scheduleForever(delay += 1.0f, 8.0f, [screenWidth, nativeAd] {
        ee::runOnUiThread([screenWidth, nativeAd] {
            getLogger().info("Move to top-right");
            int width, height;
            std::tie(width, height) = nativeAd->getSize();
            nativeAd->setPosition(screenWidth - width, 0);
        });
    });

    scheduleForever(delay += 1.0f, 8.0f, [screenWidth, screenHeight, nativeAd] {
        ee::runOnUiThread([screenWidth, screenHeight, nativeAd] {
            getLogger().info("Move to bottom-right");
            int width, height;
            std::tie(width, height) = nativeAd->getSize();
            nativeAd->setPosition(screenWidth - width, screenHeight - height);
        });
    });

    scheduleForever(delay += 1.0f, 8.0f, [screenHeight, nativeAd] {
        ee::runOnUiThread([screenHeight, nativeAd] {
            getLogger().info("Move to bottom-left");
            int width, height;
            std::tie(width, height) = nativeAd->getSize();
            nativeAd->setPosition(0, screenHeight - height);
        });
    });

    scheduleForever(delay += 1.0f, 8.0f, [screenWidth, screenHeight, nativeAd] {
        ee::runOnUiThread([screenWidth, screenHeight, nativeAd] {
            getLogger().info("Move to center");
            int width, height;
            std::tie(width, height) = nativeAd->getSize();
            nativeAd->setPosition((screenWidth - width) / 2,
                                  (screenHeight - height) / 2);
        });
    });

    scheduleForever(delay += 1.0f, 8.0f, [screenWidth, screenHeight, nativeAd] {
        ee::runOnUiThread([screenWidth, screenHeight, nativeAd] {
            getLogger().info("Resize = screen size");
            nativeAd->setPosition(0, 0);
            nativeAd->setSize(screenWidth, screenHeight);
        });
    });

    scheduleForever(delay += 1.0f, 8.0f, [screenWidth, screenHeight, nativeAd] {
        ee::runOnUiThread([screenWidth, screenHeight, nativeAd] {
            getLogger().info("Resize = screen size / 2");
            nativeAd->setPosition(screenWidth / 4, screenHeight / 4);
            nativeAd->setSize(screenWidth / 2, screenHeight / 2);
        });
    });
}

void testFacebookInterstitialAd() {
    auto interstitialAd = createFacebookInterstitialAd();
    scheduleForever(1.0f, 3.0f, [interstitialAd] {
        logCurrentThread();
        ee::runOnUiThread([interstitialAd] {
            logCurrentThread();
            interstitialAd->load();
        });
    });
    scheduleForever(2.0f, 3.0f, [interstitialAd] {
        logCurrentThread();
        ee::runOnUiThread([interstitialAd] {
            logCurrentThread();
            interstitialAd->show();
        });
    });
}
} // namespace eetest
