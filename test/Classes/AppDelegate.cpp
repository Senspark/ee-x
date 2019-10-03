//
//  AppDelegate.cpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#include "AppDelegate.hpp"
#include "AdMob.hpp"
#include "AppLovin.hpp"
#include "CrashlyticsAgent.hpp"
#include "FacebookAds.hpp"
#include "IronSource.hpp"
#include "MultiNativeAdTestScene.hpp"
#include "NotificationAgent.hpp"
#include "UnityAds.hpp"
#include "Utils.hpp"
#include "Vungle.hpp"
#include "VideoPlayerTestScene.hpp"
#include "TwitterShareTestScene.hpp"

#include <ee/Ads.hpp>
#include <ee/Core.hpp>
#include <ee/Macro.hpp>

#include <cocos2d.h>

namespace eetest
{
namespace
{
const auto DesignResolution = cocos2d::Size(480, 320);
} // namespace

namespace
{
void testMultiAds()
{
    static ee::MultiRewardedVideo *ads;
    ee::runOnUiThread([] {
        static auto temp = ee::MultiRewardedVideo();
        ads = &temp;

        // ads.addItem(getAppLovin()->createRewardedVideo());
        ads->addItem(getIronSource()->createRewardedVideo(
            getIronSourceRewardedVideoId()));
        // ads.addItem(getUnityAds()->createRewardedVideo(getUnityRewardedVideoId()));
        // ads.addItem(getVungle()->createRewardedVideo());

        ads->setResultCallback([](bool result) {
            logCurrentThread();
            getLogger().info("Result = ", result ? "succeeded" : "failed");
        });
    });

    scheduleForever(2.0f, 3.0f, [] {
        logCurrentThread();
        ee::runOnUiThread([] {
            logCurrentThread();
            ads->show();
        });
    });
}
} // namespace

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

void AppDelegate::initGLContextAttrs()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 16, 8};
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    cocos2d::GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    cocos2d::log(__PRETTY_FUNCTION__);

    auto director = cocos2d::Director::getInstance();
    auto glView = director->getOpenGLView();
    if (glView == nullptr)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || \
    (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) ||   \
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
    auto &&frameSize = glView->getFrameSize();
    getLogger().info(cocos2d::StringUtils::format(
        "frameSize = %f %f", frameSize.width, frameSize.height));

    auto &&winSize = director->getWinSize();
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

    getLogger().info("Cocos thread ID: %s", getCurrentThreadId().c_str());
    ee::runOnUiThreadAndWait([] {
        getLogger().info("UI thread ID: %s", getCurrentThreadId().c_str());
    });

    getLogger().info("SHA1: %s", ee::getSHA1CertificateFingerprint().c_str());
    getLogger().info("Version name: %s", ee::getVersionName().c_str());
    getLogger().info("Version code: %s", ee::getVersionCode().c_str());
    getLogger().info("isTablet: %s", ee::isTablet() ? "true" : "false");
    getLogger().info("isConnected: %s",
                     ee::testConnection() ? "true" : "false");

    NotificationAgent::getInstance()->initialize();
    // testAdMobBannerAd();
    // testAdMobNativeAd();
    // testAdMobInterstitial();
    // testAdMobRewardedVideo();
    // testAppLovin();
    // testUnityAdsRewardedVideo();
    // testIronSourceRewardedVideo();
    // testVungle();
    // testMultiAds();
    // testFacebookInterstitialAd();
    // testFacebookNativeAd();

    cocos2d::log("Create scene");
    // director->runWithScene(VideoPlayerTestScene::create());
    // director->runWithScene(createMultiNativeAdTestScene());
    director->runWithScene(TwitterShareTestScene::create());

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    cocos2d::log(__PRETTY_FUNCTION__);
    cocos2d::Director::getInstance()->stopAnimation();
#ifndef EE_X_DESKTOP
    NotificationAgent::getInstance()->scheduleAll();
#endif // EE_X_DESKTOP
}

void AppDelegate::applicationWillEnterForeground()
{
    cocos2d::log(__PRETTY_FUNCTION__);
    cocos2d::Director::getInstance()->startAnimation();
#ifndef EE_X_DESKTOP
    NotificationAgent::getInstance()->unscheduleAll();
#endif // EE_X_DESKTOP
}
} // namespace eetest
