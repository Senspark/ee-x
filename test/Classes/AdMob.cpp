//
//  AdMob.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "AdMob.hpp"
#include "Utils.hpp"

#include <ee/Core.hpp>

#include <base/CCDirector.h>
#include <platform/CCGLView.h>
#include <platform/CCPlatformConfig.h>

namespace eetest {
ee::AdMob* getAdMob() {
    static auto plugin = ee::AdMob();
    static bool initialized = false;
    if (not initialized) {
        ee::runOnUiThreadAndWait([] {
            plugin.addTestDevice(plugin.getEmulatorTestDeviceHash());
            plugin.addTestDevice(
                "930A5959F4325BAA45E24449B03CB221"); // BlueStacks
            plugin.addTestDevice(
                "137E2FB99476DB666A99FC3C9F585D65"); // Nexus 5.
        });
        initialized = true;
    }
    return &plugin;
}

std::string getAdMobRewardedVideoId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "ca-app-pub-3940256099942544/5224354917";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "ca-app-pub-3940256099942544/1712485313";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

std::string getAdMobNativeAdId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "ca-app-pub-2101587572072038/1175956700";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "ca-app-pub-3940256099942544/2247696110";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

std::shared_ptr<ee::IAdView> createAdMobNativeAd() {
    return getAdMob()->createNativeAd(getAdMobNativeAdId(), "admob_native_spin",
                                      ee::AdMobNativeAdLayout()
                                          .setBody("ad_body")
                                          .setCallToAction("ad_call_to_action")
                                          .setHeadline("ad_headline")
                                          .setIcon("ad_icon")
                                          .setImage("ad_image")
                                          .setMedia("ad_media")
                                          .setPrice("ad_price")
                                          .setStarRating("ad_star_rating")
                                          .setStore("ad_store"));
}

void testAdMobBannerAd() {
    auto&& frameSize =
        cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
    int screenWidth = static_cast<int>(frameSize.width);
    int screenHeight = static_cast<int>(frameSize.height);

    auto bannerAd =
        ee::runOnUiThreadAndWaitResult<std::shared_ptr<ee::IAdView>>([] {
            getLogger().info("Create AdMob banner ad begin");
            auto ad = getAdMob()->createBannerAd(
                "ca-app-pub-3940256099942544/6300978111",
                ee::AdMobBannerAdSize::Normal);
            ad->load();
            ad->setVisible(false);
            getLogger().info("Create AdMob banner ad end");
            return ad;
        });

    float delay = 0.0f;
    scheduleOnce(delay += 2.0f, [screenWidth, screenHeight, bannerAd] {
        ee::runOnUiThread([screenWidth, screenHeight, bannerAd] {
            getLogger().info("show");
            bannerAd->setVisible(true);

            int width, height;
            std::tie(width, height) = bannerAd->getSize();
            bannerAd->setPosition((screenWidth - width) / 2,
                                  (screenHeight - height) / 2);
        });
    });

    scheduleForever(delay += 2.0f, 8.0f, [screenWidth, screenHeight, bannerAd] {
        ee::runOnUiThread([screenWidth, screenHeight, bannerAd] {
            getLogger().info("Move to top-left");
            bannerAd->setPosition(0, 0);
        });
    });

    scheduleForever(delay += 2.0f, 8.0f, [screenWidth, screenHeight, bannerAd] {
        ee::runOnUiThread([screenWidth, screenHeight, bannerAd] {
            getLogger().info("Move to top-right");
            int width, height;
            std::tie(width, height) = bannerAd->getSize();
            bannerAd->setPosition(screenWidth - width, 0);
        });
    });

    scheduleForever(delay += 2.0f, 8.0f, [screenWidth, screenHeight, bannerAd] {
        ee::runOnUiThread([screenWidth, screenHeight, bannerAd] {
            getLogger().info("Move to bottom-right");
            int width, height;
            std::tie(width, height) = bannerAd->getSize();
            bannerAd->setPosition(screenWidth - width, screenHeight - height);
        });
    });

    scheduleForever(delay += 2.0f, 8.0f, [screenWidth, screenHeight, bannerAd] {
        ee::runOnUiThread([screenWidth, screenHeight, bannerAd] {
            getLogger().info("Move to bottom-left");
            int width, height;
            std::tie(width, height) = bannerAd->getSize();
            bannerAd->setPosition(0, screenHeight - height);
        });
    });
}

void testAdMobNativeAd() {
    auto&& frameSize =
        cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
    int screenWidth = static_cast<int>(frameSize.width);
    int screenHeight = static_cast<int>(frameSize.height);

    auto nativeAd =
        ee::runOnUiThreadAndWaitResult<std::shared_ptr<ee::IAdView>>([] {
            getLogger().info("Create AdMob native ad begin");
            auto ad = createAdMobNativeAd();
            ad->setVisible(true);
            ad->setSize(600, 100);
            getLogger().info("Create AdMob native ad end");
            return ad;
        });

    float delay = 0.0f;
    scheduleForever(delay + 1.0f, 4.0f, [nativeAd] {
        ee::runOnUiThread([nativeAd] {
            getLogger().info("Load AdMob native ad");
            nativeAd->load();
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight, nativeAd] {
        ee::runOnUiThread([screenWidth, screenHeight, nativeAd] {
            getLogger().info("Resize = screen size / 4");
            nativeAd->setPosition(3 * screenWidth / 8, 3 * screenHeight / 8);
            nativeAd->setSize(screenWidth / 4, screenHeight / 4);
        });
    });

    scheduleOnce(delay += 1.0f, [nativeAd] {
        ee::runOnUiThread([nativeAd] {
            getLogger().info("Move to top-left");
            nativeAd->setPosition(0, 0);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, nativeAd] {
        ee::runOnUiThread([screenWidth, nativeAd] {
            getLogger().info("Move to top-right");
            int width, height;
            std::tie(width, height) = nativeAd->getSize();
            nativeAd->setPosition(screenWidth - width, 0);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight, nativeAd] {
        ee::runOnUiThread([screenWidth, screenHeight, nativeAd] {
            getLogger().info("Move to bottom-right");
            int width, height;
            std::tie(width, height) = nativeAd->getSize();
            nativeAd->setPosition(screenWidth - width, screenHeight - height);
        });
    });

    scheduleOnce(delay += 1.0f, [screenHeight, nativeAd] {
        ee::runOnUiThread([screenHeight, nativeAd] {
            getLogger().info("Move to bottom-left");
            int width, height;
            std::tie(width, height) = nativeAd->getSize();
            nativeAd->setPosition(0, screenHeight - height);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight, nativeAd] {
        ee::runOnUiThread([screenWidth, screenHeight, nativeAd] {
            getLogger().info("Move to center");
            int width, height;
            std::tie(width, height) = nativeAd->getSize();
            nativeAd->setPosition((screenWidth - width) / 2,
                                  (screenHeight - height) / 2);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight, nativeAd] {
        ee::runOnUiThread([screenWidth, screenHeight, nativeAd] {
            getLogger().info("Resize = screen size");
            nativeAd->setPosition(0, 0);
            nativeAd->setSize(screenWidth, screenHeight);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight, nativeAd] {
        ee::runOnUiThread([screenWidth, screenHeight, nativeAd] {
            getLogger().info("Resize = screen size / 2");
            nativeAd->setPosition(screenWidth / 4, screenHeight / 4);
            nativeAd->setSize(screenWidth / 2, screenHeight / 2);
        });
    });
}

void testAdMobInterstitial() {
    auto interstitialAd =
        ee::runOnUiThreadAndWaitResult<std::shared_ptr<ee::IInterstitialAd>>(
            [] {
                getLogger().info("Create AdMob interstitial ad begin");
                auto ad = getAdMob()->createInterstitialAd(
                    "ca-app-pub-3940256099942544/1033173712");
                getLogger().info("Create AdMob interstitial ad end");
                return ad;
            });

    scheduleForever(1.0f, 3.0f, [interstitialAd] {
        ee::runOnUiThread([interstitialAd] {
            getLogger().info("Load AdMob interstitial ad");
            interstitialAd->load();
        });
    });
    scheduleForever(2.0f, 3.0f, [interstitialAd] {
        ee::runOnUiThread([interstitialAd] {
            getLogger().info("Show AdMob interstitial ad");
            interstitialAd->show();
        });
    });
}

void testAdMobRewardedVideo() {
    auto rewardedVideo =
        ee::runOnUiThreadAndWaitResult<std::shared_ptr<ee::IRewardedVideo>>([] {
            getLogger().info("Create AdMob rewarded video begin");
            auto ad =
                getAdMob()->createRewardedVideo(getAdMobRewardedVideoId());
            getLogger().info("Create AdMob rewarded video end");
            return ad;
        });

    scheduleForever(1.0f, 3.0f, [rewardedVideo] {
        ee::runOnUiThread([rewardedVideo] {
            getLogger().info("Load AdMob rewarded video");
            rewardedVideo->load();
        });
    });

    scheduleForever(2.0f, 3.0f, [rewardedVideo] {
        ee::runOnUiThread([rewardedVideo] {
            getLogger().info("Show AdMob rewarded video");
            rewardedVideo->show();
        });
    });
}
} // namespace eetest
