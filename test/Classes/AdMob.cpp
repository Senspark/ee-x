//
//  AdMob.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "AdMob.hpp"

#include <base/CCDirector.h>
#include <platform/CCGLView.h>
#include <platform/CCPlatformConfig.h>

#include <ee/Coroutine.hpp>

#include "Utils.hpp"

namespace eetest {
ee::AdMob* getAdMob() {
    static auto plugin = ee::AdMob();
    static bool initialized = false;
    if (not initialized) {
        ee::runOnUiThreadAndWait([] {
            plugin.initialize(getAdMobApplicationTestId());
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

std::string getAdMobApplicationTestId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "ca-app-pub-3940256099942544~3347511713";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "ca-app-pub-3940256099942544~1458002511";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

std::string getAdMobBannerAdTestId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "ca-app-pub-3940256099942544/6300978111";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "ca-app-pub-3940256099942544/2934735716";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

std::string getAdMobNativeAdTestId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "ca-app-pub-3940256099942544/2247696110";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "ca-app-pub-3940256099942544/3986624511";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

std::string getAdMobNativeAdVideoTestId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "ca-app-pub-3940256099942544/1044960115";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "ca-app-pub-3940256099942544/2521693316";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

std::string getAdMobInterstitialAdTestId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "ca-app-pub-3940256099942544/1033173712";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "ca-app-pub-3940256099942544/4411468910";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

std::string getAdMobInterstitialAdVideoTestId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "ca-app-pub-3940256099942544/8691691433";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "ca-app-pub-3940256099942544/5135589807";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

std::string getAdMobRewardedAdTestId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "ca-app-pub-3940256099942544/5224354917";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "ca-app-pub-3940256099942544/1712485313";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

std::shared_ptr<ee::IAdView> createAdMobNativeAd() {
    return getAdMob()->createNativeAd(getAdMobNativeAdTestId(),
                                      "admob_native_spin",
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

    auto ad = ee::runOnUiThreadAndWaitResult<std::shared_ptr<ee::IAdView>>([] {
        getLogger().info("Create AdMob banner ad begin");
        auto ad = getAdMob()->createBannerAd(getAdMobBannerAdTestId(),
                                             ee::AdMobBannerAdSize::Normal);
        ee::noAwait([ad]() -> ee::Task<> { //
            co_await ad->load();
        });
        ad->setVisible(false);
        getLogger().info("Create AdMob banner ad end");
        return ad;
    });

    float delay = 0.0f;
    scheduleOnce(delay += 2.0f, [screenWidth, screenHeight, ad] {
        ee::runOnUiThread([screenWidth, screenHeight, ad] {
            getLogger().info("show");
            ad->setVisible(true);

            auto [width, height] = ad->getSize();
            ad->setPosition((screenWidth - width) / 2,
                            (screenHeight - height) / 2);
        });
    });

    scheduleForever(delay += 2.0f, 8.0f, [screenWidth, screenHeight, ad] {
        ee::runOnUiThread([screenWidth, screenHeight, ad] {
            getLogger().info("Move to top-left");
            ad->setPosition(0, 0);
        });
    });

    scheduleForever(delay += 2.0f, 8.0f, [screenWidth, screenHeight, ad] {
        ee::runOnUiThread([screenWidth, screenHeight, ad] {
            getLogger().info("Move to top-right");
            auto [width, height] = ad->getSize();
            ad->setPosition(screenWidth - width, 0);
        });
    });

    scheduleForever(delay += 2.0f, 8.0f, [screenWidth, screenHeight, ad] {
        ee::runOnUiThread([screenWidth, screenHeight, ad] {
            getLogger().info("Move to bottom-right");
            auto [width, height] = ad->getSize();
            ad->setPosition(screenWidth - width, screenHeight - height);
        });
    });

    scheduleForever(delay += 2.0f, 8.0f, [screenWidth, screenHeight, ad] {
        ee::runOnUiThread([screenWidth, screenHeight, ad] {
            getLogger().info("Move to bottom-left");
            auto [width, height] = ad->getSize();
            ad->setPosition(0, screenHeight - height);
        });
    });
}

void testAdMobNativeAd() {
    auto&& frameSize =
        cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
    int screenWidth = static_cast<int>(frameSize.width);
    int screenHeight = static_cast<int>(frameSize.height);

    auto ad = ee::runOnUiThreadAndWaitResult<std::shared_ptr<ee::IAdView>>([] {
        getLogger().info("Create AdMob native ad begin");
        auto ad = createAdMobNativeAd();
        ad->setVisible(true);
        ad->setSize(600, 100);
        getLogger().info("Create AdMob native ad end");
        return ad;
    });

    float delay = 0.0f;
    scheduleForever(delay + 1.0f, 4.0f, [ad] {
        ee::runOnUiThread(ee::makeAwaiter([ad]() -> ee::Task<> {
            getLogger().info("Load AdMob native ad");
            co_await ad->load();
        }));
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight, ad] {
        ee::runOnUiThread([screenWidth, screenHeight, ad] {
            getLogger().info("Resize = screen size / 4");
            ad->setPosition(3 * screenWidth / 8, 3 * screenHeight / 8);
            ad->setSize(screenWidth / 4, screenHeight / 4);
        });
    });

    scheduleOnce(delay += 1.0f, [ad] {
        ee::runOnUiThread([ad] {
            getLogger().info("Move to top-left");
            ad->setPosition(0, 0);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, ad] {
        ee::runOnUiThread([screenWidth, ad] {
            getLogger().info("Move to top-right");
            auto [width, height] = ad->getSize();
            ad->setPosition(screenWidth - width, 0);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight, ad] {
        ee::runOnUiThread([screenWidth, screenHeight, ad] {
            getLogger().info("Move to bottom-right");
            auto [width, height] = ad->getSize();
            ad->setPosition(screenWidth - width, screenHeight - height);
        });
    });

    scheduleOnce(delay += 1.0f, [screenHeight, ad] {
        ee::runOnUiThread([screenHeight, ad] {
            getLogger().info("Move to bottom-left");
            auto [width, height] = ad->getSize();
            ad->setPosition(0, screenHeight - height);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight, ad] {
        ee::runOnUiThread([screenWidth, screenHeight, ad] {
            getLogger().info("Move to center");
            auto [width, height] = ad->getSize();
            ad->setPosition((screenWidth - width) / 2,
                            (screenHeight - height) / 2);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight, ad] {
        ee::runOnUiThread([screenWidth, screenHeight, ad] {
            getLogger().info("Resize = screen size");
            ad->setPosition(0, 0);
            ad->setSize(screenWidth, screenHeight);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight, ad] {
        ee::runOnUiThread([screenWidth, screenHeight, ad] {
            getLogger().info("Resize = screen size / 2");
            ad->setPosition(screenWidth / 4, screenHeight / 4);
            ad->setSize(screenWidth / 2, screenHeight / 2);
        });
    });
}

void testAdMobInterstitial() {
    auto ad =
        ee::runOnUiThreadAndWaitResult<std::shared_ptr<ee::IInterstitialAd>>(
            [] {
                getLogger().info("Create AdMob interstitial ad begin");
                auto ad = getAdMob()->createInterstitialAd(
                    getAdMobInterstitialAdTestId());
                getLogger().info("Create AdMob interstitial ad end");
                return ad;
            });

    scheduleForever(1.0f, 3.0f, [ad] {
        ee::runOnUiThread(ee::makeAwaiter([ad]() -> ee::Task<> {
            getLogger().info("Load AdMob interstitial ad");
            co_await ad->load();
        }));
    });
    scheduleForever(2.0f, 3.0f, [ad] {
        ee::runOnUiThread(ee::makeAwaiter([ad]() -> ee::Task<> {
            getLogger().info("Show AdMob interstitial ad");
            co_await ad->show();
        }));
    });
}

void testAdMobRewardedVideo() {
    auto ad =
        ee::runOnUiThreadAndWaitResult<std::shared_ptr<ee::IRewardedAd>>([] {
            getLogger().info("Create AdMob rewarded ad begin");
            auto ad = getAdMob()->createRewardedAd(getAdMobRewardedAdTestId());
            getLogger().info("Create AdMob rewarded ad end");
            return ad;
        });

    scheduleForever(1.0f, 3.0f, [ad] {
        ee::runOnUiThread(ee::makeAwaiter([ad]() -> ee::Task<> {
            getLogger().info("Load AdMob rewarded ad");
            co_await ad->load();
        }));
    });

    scheduleForever(2.0f, 3.0f, [ad] {
        ee::runOnUiThread(ee::makeAwaiter([ad]() -> ee::Task<> {
            getLogger().info("Show AdMob rewarded ad");
            co_await ad->show();
        }));
    });
}
} // namespace eetest
