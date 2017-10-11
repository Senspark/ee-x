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

ee::FacebookAds* facebookAds_;
std::shared_ptr<ee::AdViewInterface> facebookNativeAd_;

void testFacebookNativeAd() {
    auto&& frameSize =
        cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
    int screenWidth = static_cast<int>(frameSize.width);
    int screenHeight = static_cast<int>(frameSize.height);

    float delay = 0.0f;
    scheduleOnce(delay += 1.0f, [] {
        getLogger().info("Create FacebookAds plugin");
        static auto plugin = ee::FacebookAds();
        facebookAds_ = &plugin;
        facebookAds_->clearTestDevices();
        facebookAds_->addTestDevice(facebookAds_->getTestDeviceHash());
        facebookAds_->addTestDevice(
            "ad45c323f6a9b07f7a9c072549efb279"); // BlueStacks.
    });

    scheduleOnce(delay += 1.0f, [] {
        ee::runOnUiThread([] {
            getLogger().info("Create Facebook native ad");
            facebookNativeAd_ = facebookAds_->createNativeAd(
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
            facebookNativeAd_->setVisible(true);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight] {
        ee::runOnUiThread([screenWidth, screenHeight] {
            getLogger().info("Resize = screen size / 4");
            facebookNativeAd_->setPosition(3 * screenWidth / 8,
                                           3 * screenHeight / 8);
            facebookNativeAd_->setSize(screenWidth / 4, screenHeight / 4);
        });
    });

    scheduleOnce(delay += 1.0f, [] {
        ee::runOnUiThread([] {
            getLogger().info("Move to top-left");
            facebookNativeAd_->setPosition(0, 0);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth] {
        ee::runOnUiThread([screenWidth] {
            getLogger().info("Move to top-right");
            int width, height;
            std::tie(width, height) = facebookNativeAd_->getSize();
            facebookNativeAd_->setPosition(screenWidth - width, 0);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight] {
        ee::runOnUiThread([screenWidth, screenHeight] {
            getLogger().info("Move to bottom-right");
            int width, height;
            std::tie(width, height) = facebookNativeAd_->getSize();
            facebookNativeAd_->setPosition(screenWidth - width,
                                           screenHeight - height);
        });
    });

    scheduleOnce(delay += 1.0f, [screenHeight] {
        ee::runOnUiThread([screenHeight] {
            getLogger().info("Move to bottom-left");
            int width, height;
            std::tie(width, height) = facebookNativeAd_->getSize();
            facebookNativeAd_->setPosition(0, screenHeight - height);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight] {
        ee::runOnUiThread([screenWidth, screenHeight] {
            getLogger().info("Move to center");
            int width, height;
            std::tie(width, height) = facebookNativeAd_->getSize();
            facebookNativeAd_->setPosition((screenWidth - width) / 2,
                                           (screenHeight - height) / 2);
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight] {
        ee::runOnUiThread([screenWidth, screenHeight] {
            getLogger().info("Resize = screen size");
            facebookNativeAd_->setPosition(0, 0);
            facebookNativeAd_->setSize(screenWidth, screenHeight);
            facebookNativeAd_->load();
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight] {
        ee::runOnUiThread([screenWidth, screenHeight] {
            getLogger().info("Resize = screen size / 2");
            facebookNativeAd_->setPosition(screenWidth / 4, screenHeight / 4);
            facebookNativeAd_->setSize(screenWidth / 2, screenHeight / 2);
        });
    });
}

void testAppLovin() {
    getLogger().info("Create AppLovin plugin");

    static auto plugin = ee::AppLovin();
    static auto appLovin = &plugin;
    appLovin = &plugin;
     appLovin->initialize(
         R"(gG8pkErh1_fQo-4cNDXGnxGyb9H4qz6VDEJyS8eU8IvxH-XeB4wy0BubKAg97neL0yIT4xyDEs8WqfA0l4zlGr)");
    // appLovin->initialize(
    //     R"(S9_dC7VN_TzZ700tTmRpGQVgoXZyIbOmlOOyVvJVRH3TI4PmqrT5G3m2bJ_uwNefn2bHxwnMBqwTvKEi9ooPrX)");
    appLovin->setTestAdsEnabled(true);
    appLovin->setVerboseLogging(true);

    static auto rewardedVideo = appLovin->createRewardedVideo();
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

void testUnityAds() {
    getLogger().info("Create UnityAds plugin");
    static auto plugin = ee::UnityAds();
    static auto unityAds_ = &plugin;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    constexpr auto gameId = "73406";
    constexpr auto rewardedVideoId = "rewardedVideoZone";
    constexpr auto interstitialAdId = "defaultZone";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    constexpr auto gameId = "1423604";
    constexpr auto interstitialAdId = "video";
    constexpr auto rewardedVideoId = "rewardedVideo";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

    unityAds_->initialize(gameId, false);

    static auto rewardedVideo = unityAds_->createRewardedVideo(rewardedVideoId);
    rewardedVideo->setResultCallback([](bool result) {
        logCurrentThread();
        getLogger().info("Result = ", result ? "succeeded" : "failed");
    });

    static auto interstitialAd =
        unityAds_->createInterstitialAd(interstitialAdId);
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

void testIronSource() {
    getLogger().info("Create IronSource plugin");
    static auto plugin = ee::IronSource();
    static auto ironSource = &plugin;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    constexpr auto gameId = "67a6443d";
    constexpr auto rewardedVideoId = "DefaultRewardedVideo";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    constexpr auto gameId = "67a60ab5";
    constexpr auto rewardedVideoId = "DefaultRewardedVideo";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

    ironSource->initialize(gameId);

    static auto rewardedVideo =
        ironSource->createRewardedVideo(rewardedVideoId);

    float delay = 0.0f;
    scheduleOnce(delay += 5.0f, [] {
        getLogger().info("Show IronSource rewarded video");
        rewardedVideo->show();
    });
}

void testVungle() {
    getLogger().info("Create Vungle plugin");
    static auto plugin = ee::Vungle();
    static auto vungle = &plugin;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    constexpr auto gameId = "com.senspark.goldminerclassic";
    constexpr auto rewardedVideoId = "rewarded";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    constexpr auto gameId = "651916412";
    constexpr auto rewardedVideoId = "rewarded";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

    vungle->initialize(gameId);

    static auto rewardedVideo = vungle->createRewardedVideo();
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

    testAppLovin();
    // testUnityAds();
    // testIronSource();
    // testVungle();

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
