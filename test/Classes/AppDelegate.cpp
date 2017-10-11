//
//  AppDelegate.cpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#include "AppDelegate.hpp"
#include "CrashlyticsAgent.hpp"
#include "NotificationAgent.hpp"

#include <cocos2d.h>

#include <ee/Ads.hpp>
#include <ee/AppLovin.hpp>
#include <ee/Core.hpp>
#include <ee/FacebookAds.hpp>
#include <ee/IronSource.hpp>
#include <ee/Macro.hpp>
#include <ee/UnityAds.hpp>
#include <ee/Vungle.hpp>

namespace {
const auto DesignResolution = cocos2d::Size(480, 320);
} // namespace

namespace {
ee::Logger& getLogger() {
    static ee::Logger logger("ee_x");
    return logger;
}

std::string getCurrentThreadId() {
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return ss.str();
}

void logCurrentThread() {
    getLogger().info("Current thread ID: ", getCurrentThreadId());
}

void schedule(float delay, float interval, const std::function<void()>& f) {
    static int target;
    static int counter;
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->schedule(std::bind(f), &target, interval, CC_REPEAT_FOREVER,
                        delay, false, std::to_string(counter++));
}

void scheduleOnce(float delay, const std::function<void()>& f) {
    static int target;
    static int counter;
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->schedule(std::bind(f), &target, 0, 0, delay, false,
                        std::to_string(counter++));
}

ee::FacebookAds* getFacebookAds() {
    static auto plugin = ee::FacebookAds();
    static bool initialized = false;
    if (not initialized) {
        initialized = true;
        plugin.clearTestDevices();
        plugin.addTestDevice(plugin.getTestDeviceHash());
        plugin.addTestDevice("ad45c323f6a9b07f7a9c072549efb279"); // BlueStacks.
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

void testFacebookNativeAd() {
    auto&& frameSize =
        cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
    int screenWidth = static_cast<int>(frameSize.width);
    int screenHeight = static_cast<int>(frameSize.height);

    static std::shared_ptr<ee::AdViewInterface> nativeAd;

    float delay = 0.0f;
    scheduleOnce(delay += 1.0f, [] {
        getLogger().info("Create FacebookAds plugin");
        getFacebookAds();
    });

    scheduleOnce(delay += 1.0f, [] {
        ee::runOnUiThread([] {
            getLogger().info("Create Facebook native ad");
            nativeAd = getFacebookAds()->createNativeAd(
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
            nativeAd->setVisible(true);
            nativeAd->load();
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight] {
        ee::runOnUiThread([screenWidth, screenHeight] {
            getLogger().info("Resize = screen size / 4");
            nativeAd->setPosition(3 * screenWidth / 8, 3 * screenHeight / 8);
            nativeAd->setSize(screenWidth / 4, screenHeight / 4);
        });
    });

    scheduleOnce(delay += 1.0f, [] {
        ee::runOnUiThread([] {
            getLogger().info("Move to top-left");
            nativeAd->setPosition(0, 0);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth] {
        ee::runOnUiThread([screenWidth] {
            getLogger().info("Move to top-right");
            int width, height;
            std::tie(width, height) = nativeAd->getSize();
            nativeAd->setPosition(screenWidth - width, 0);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight] {
        ee::runOnUiThread([screenWidth, screenHeight] {
            getLogger().info("Move to bottom-right");
            int width, height;
            std::tie(width, height) = nativeAd->getSize();
            nativeAd->setPosition(screenWidth - width, screenHeight - height);
        });
    });

    scheduleOnce(delay += 1.0f, [screenHeight] {
        ee::runOnUiThread([screenHeight] {
            getLogger().info("Move to bottom-left");
            int width, height;
            std::tie(width, height) = nativeAd->getSize();
            nativeAd->setPosition(0, screenHeight - height);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight] {
        ee::runOnUiThread([screenWidth, screenHeight] {
            getLogger().info("Move to center");
            int width, height;
            std::tie(width, height) = nativeAd->getSize();
            nativeAd->setPosition((screenWidth - width) / 2,
                                  (screenHeight - height) / 2);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight] {
        ee::runOnUiThread([screenWidth, screenHeight] {
            getLogger().info("Resize = screen size");
            nativeAd->setPosition(0, 0);
            nativeAd->setSize(screenWidth, screenHeight);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight] {
        ee::runOnUiThread([screenWidth, screenHeight] {
            getLogger().info("Resize = screen size / 2");
            nativeAd->setPosition(screenWidth / 4, screenHeight / 4);
            nativeAd->setSize(screenWidth / 2, screenHeight / 2);
        });
    });
}

void testFacebookInterstitialAd() {
    static auto interstitialAd =
        getFacebookAds()->createInterstitialAd(getFacebookInterstitialAdId());
    schedule(1.0f, 3.0f, [] {
        logCurrentThread();
        ee::runOnUiThread([] {
            logCurrentThread();
            interstitialAd->load();
        });
    });
    schedule(2.0f, 3.0f, [] {
        logCurrentThread();
        ee::runOnUiThread([] {
            logCurrentThread();
            interstitialAd->show();
        });
    });
}

ee::AppLovin* getAppLovin() {
    static auto plugin = ee::AppLovin();
    static bool initialized;
    if (not initialized) {
        initialized = true;
        plugin.initialize(
            R"(gG8pkErh1_fQo-4cNDXGnxGyb9H4qz6VDEJyS8eU8IvxH-XeB4wy0BubKAg97neL0yIT4xyDEs8WqfA0l4zlGr)");
        plugin.setTestAdsEnabled(true);
        plugin.setVerboseLogging(true);
    }
    return &plugin;
}

void testAppLovin() {
    static auto rewardedVideo = getAppLovin()->createRewardedVideo();
    rewardedVideo->setResultCallback([](bool result) {
        logCurrentThread();
        getLogger().info("Result = ", result ? "succeeded" : "failed");
    });

    float delay = 0.0f;
    schedule(delay += 1.0f, 3.0f, [] {
        getLogger().info("Load AppLovin rewarded video");
        rewardedVideo->load();
    });
    schedule(delay += 1.0f, 3.0f, [] {
        getLogger().info("Show AppLovin rewarded video");
        rewardedVideo->show();
    });
}

ee::UnityAds* getUnityAds() {
    static auto plugin = ee::UnityAds();
    static bool initialized;
    if (not initialized) {
        initialized = true;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        constexpr auto gameId = "73406";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        constexpr auto gameId = "1423604";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        plugin.initialize(gameId, false);
    }
    return &plugin;
}

std::string getUnityRewardedVideoId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "rewardedVideoZone";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "rewardedVideo";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

std::string getUnityInterstitialAdId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "defaultZone";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "video";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

void testUnityAds() {
    static auto rewardedVideo =
        getUnityAds()->createRewardedVideo(getUnityRewardedVideoId());
    rewardedVideo->setResultCallback([](bool result) {
        logCurrentThread();
        getLogger().info("Result = ", result ? "succeeded" : "failed");
    });

    static auto interstitialAd =
        getUnityAds()->createInterstitialAd(getUnityInterstitialAdId());
    interstitialAd->setResultCallback([] {
        logCurrentThread();
        getLogger().info("Done");
    });

    float delay = 0.0f;
    scheduleOnce(delay += 5.0f, [] {
        getLogger().info("Show UnityAds rewarded video");
        rewardedVideo->show();
    });

    scheduleOnce(delay += 2.0f, [] {
        getLogger().info("Show UnityAds interstitial ad");
        interstitialAd->show();
    });
}

ee::IronSource* getIronSource() {
    static auto plugin = ee::IronSource();
    static bool initialized;
    if (not initialized) {
        initialized = true;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        constexpr auto gameId = "67a6443d";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        constexpr auto gameId = "67a60ab5";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        plugin.initialize(gameId);
    }
    return &plugin;
}

std::string getIronSourceRewardedVideoId() {
    return "DefaultRewardedVideo";
}

void testIronSource() {
    static auto rewardedVideo =
        getIronSource()->createRewardedVideo(getIronSourceRewardedVideoId());

    float delay = 0.0f;
    scheduleOnce(delay += 5.0f, [] {
        getLogger().info("Show IronSource rewarded video");
        rewardedVideo->show();
    });
}

ee::Vungle* getVungle() {
    static auto plugin = ee::Vungle();
    static bool initialized;
    if (not initialized) {
        initialized = true;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        constexpr auto gameId = "com.senspark.goldminerclassic";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        constexpr auto gameId = "651916412";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        plugin.initialize(gameId);
    }
    return &plugin;
}

std::string getVungleRewardedVideoId() {
    return "rewarded";
}

void testVungle() {
    static auto rewardedVideo = getVungle()->createRewardedVideo();
    rewardedVideo->setResultCallback([](bool result) {
        logCurrentThread();
        getLogger().info("Result = ", result ? "succeeded" : "failed");
    });

    float delay = 0.0f;
    schedule(delay += 5.0f, 5.0f, [] {
        getLogger().info("Show Vungle rewarded video");
        rewardedVideo->show();
    });
}

void testMultiAds() {
    static auto ads = ee::MultiRewardedVideo();
    ads.addItem(getAppLovin()->createRewardedVideo())
        .addItem(getIronSource()->createRewardedVideo(
            getIronSourceRewardedVideoId()))
        .addItem(getUnityAds()->createRewardedVideo(getUnityRewardedVideoId()))
        .addItem(getVungle()->createRewardedVideo());

    ads.setResultCallback([](bool result) {
        logCurrentThread();
        getLogger().info("Result = ", result ? "succeeded" : "failed");
    });
    schedule(2.0f, 3.0f, [] {
        logCurrentThread();
        ads.show();
    });
}
} // namespace

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

void AppDelegate::initGLContextAttrs() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 16, 8};
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    cocos2d::GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    cocos2d::log(__PRETTY_FUNCTION__);

    auto director = cocos2d::Director::getInstance();
    auto glView = director->getOpenGLView();
    if (glView == nullptr) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) ||                               \
    (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) ||                                 \
    (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glView = cocos2d::GLViewImpl::createWithRect(
            "HelloCpp", cocos2d::Rect(0, 0, DesignResolution.width,
                                      DesignResolution.height));
#else
        glView = cocos2d::GLViewImpl::create("HelloCpp");
#endif
        director->setOpenGLView(glView);
    }

    director->setDisplayStats(true);
    director->setAnimationInterval(1.0f / 60);

    auto resolutionPolicy = (DesignResolution.height > DesignResolution.width
                                 ? ResolutionPolicy::FIXED_WIDTH
                                 : ResolutionPolicy::FIXED_HEIGHT);
    glView->setDesignResolutionSize(DesignResolution.width,
                                    DesignResolution.height, resolutionPolicy);
    auto&& frameSize = glView->getFrameSize();
    getLogger().info(cocos2d::StringUtils::format(
        "frameSize = %f %f", frameSize.width, frameSize.height));

    auto&& winSize = director->getWinSize();
    getLogger().info(cocos2d::StringUtils::format(
        "winSize = %f %f", winSize.width, winSize.height));

    ee::Metrics::initialize(frameSize.height / winSize.height);
    constexpr float points = 1;
    auto metrics = ee::Metrics::fromPoint(points);
    auto dp = metrics.toDip();
    auto pixels = metrics.toPixel();
    getLogger().info(cocos2d::StringUtils::format("%f pt = %f pixels = %f dp",
                                                  points, pixels, dp));

    CrashlyticsAgent::getInstance()->initialize();
    CrashlyticsAgent::getInstance()->logDebug("debug_message");
    CrashlyticsAgent::getInstance()->logInfo("info_message");
    CrashlyticsAgent::getInstance()->logError("error_message");
    CrashlyticsAgent::getInstance()->trackLevelStart("level_test");
    CrashlyticsAgent::getInstance()->trackLevelEnd("level_test", 100, true);
    CrashlyticsAgent::getInstance()->trackPurchase(
        100.f, "USD", true, "Item Test", "Test Type", "TEST_ID");

    CrashlyticsAgent::CustomAttributesType attrs;
    attrs["title"] = "Beautiful in white";
    attrs["singer"] = "Westlife";
    CrashlyticsAgent::getInstance()->trackCustomEvent("PlaySong", attrs);
    CrashlyticsAgent::getInstance()->trackInvite("Twitter");

    getLogger().info("Cocos thread ID: ", getCurrentThreadId());
    ee::runOnUiThread(
        [] { getLogger().info("UI thread ID: ", getCurrentThreadId()); });

    // testAppLovin();
    // testUnityAds();
    // testIronSource();
    // testVungle();
    // testMultiAds();
    testFacebookInterstitialAd();

    cocos2d::log("Create scene");
    auto scene = cocos2d::Scene::create();
    auto layer =
        cocos2d::LayerColor::create(cocos2d::Color4B(150, 150, 150, 150));
    scene->addChild(layer);
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    cocos2d::log(__PRETTY_FUNCTION__);
    cocos2d::Director::getInstance()->stopAnimation();
#ifndef EE_X_DESKTOP
    NotificationAgent::getInstance()->scheduleAll();
#endif // EE_X_DESKTOP
}

void AppDelegate::applicationWillEnterForeground() {
    cocos2d::log(__PRETTY_FUNCTION__);
    cocos2d::Director::getInstance()->startAnimation();
#ifndef EE_X_DESKTOP
    NotificationAgent::getInstance()->unscheduleAll();
#endif // EE_X_DESKTOP
}
