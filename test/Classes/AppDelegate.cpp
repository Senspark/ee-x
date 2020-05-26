//
//  AppDelegate.cpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#include "AppDelegate.hpp"

#include <cocos2d.h>

#include <ee/Ads.hpp>
#include <ee/Cocos.hpp>

#include "AdMob.hpp"
#include "AppLovin.hpp"
#include "CrashlyticsAgent.hpp"
#include "FacebookAds.hpp"
#include "IronSource.hpp"
#include "MultiNativeAdTestScene.hpp"
#include "NotificationAgent.hpp"
#include "TwitterShareTestScene.hpp"
#include "UnityAds.hpp"
#include "Utils.hpp"
#include "VideoPlayerTestScene.hpp"
#include "Vungle.hpp"

namespace eetest {
namespace {
const auto DesignResolution = cocos2d::Size(480, 320);
} // namespace

namespace {
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
// Nothing.
#else
void testMultiAds() {
    auto ad = std::make_shared<ee::MultiRewardedAd>();

    ee::runOnUiThread([ad] {
        ad->addItem(std::make_shared<ee::GuardedRewardedAd>(
            getAdMob()->createRewardedAd(getAdMobRewardedAdTestId())));
        ad->addItem(std::make_shared<ee::GuardedRewardedAd>(
            getAppLovin()->createRewardedAd()));
        ad->addItem(std::make_shared<ee::GuardedRewardedAd>(
            getIronSource()->createRewardedAd(getIronSourceRewardedAdId())));
        ad->addItem(std::make_shared<ee::GuardedRewardedAd>(
            getUnityAds()->createRewardedAd(getUnityRewardedAdId())));
        ad->addItem(std::make_shared<ee::GuardedRewardedAd>(
            getVungle()->createRewardedAd(getVungleRewardedAdId())));
    });

    scheduleForever(1.0f, 3.0f, [ad] {
        logCurrentThread();
        ee::runOnUiThread(ee::makeAwaiter([ad]() -> ee::Task<> {
            logCurrentThread();
            getLogger().info("Load rewarded ad");
            auto result = co_await ad->load();
            logCurrentThread();
            getLogger().info("Load rewarded ad: result = %d",
                             static_cast<int>(result));
        }));
    });

    scheduleForever(2.0f, 3.0f, [ad] {
        logCurrentThread();
        ee::runOnUiThread(ee::makeAwaiter([ad]() -> ee::Task<> {
            logCurrentThread();
            getLogger().info("Show rewarded ad");
            auto result = co_await ad->show();
            logCurrentThread();
            getLogger().info("Show rewarded ad result = %d",
                             static_cast<int>(result));
        }));
    });
}
#endif
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
    getLogger().info("frameSize = %f %f", frameSize.width, frameSize.height);

    auto&& winSize = director->getWinSize();
    getLogger().info("winSize = %f %f", winSize.width, winSize.height);

    constexpr float points = 1;
    auto metrics = ee::Metrics::fromPoint(points);
    auto dp = metrics.toDip();
    auto pixels = metrics.toPixel();
    getLogger().info("%f pt = %f pixels = %f dp", points, pixels, dp);

    ee::Logger::setSystemLogger(getLogger());

    // CrashlyticsAgent::getInstance()->initialize();
    // CrashlyticsAgent::getInstance()->logDebug("debug_message");
    // CrashlyticsAgent::getInstance()->logInfo("info_message");

    getLogger().info("Cocos thread ID: %s", getCurrentThreadId().c_str());
    ee::runOnUiThreadAndWait([] {
        getLogger().info("UI thread ID: %s", getCurrentThreadId().c_str());
    });

    getLogger().info("SHA1: %s", ee::getSHA1CertificateFingerprint().c_str());
    getLogger().info("Version name: %s", ee::getVersionName().c_str());
    getLogger().info("Version code: %s", ee::getVersionCode().c_str());
    getLogger().info("isTablet: %s", ee::isTablet() ? "true" : "false");

    ee::noAwait([]() -> ee::Task<> {
        auto isConnected = co_await ee::testConnection("www.google.com", 1.0f);
        getLogger().info("isConnected: %s", isConnected ? "true" : "false");
    });

    // NotificationAgent::getInstance()->initialize();
    // testAdMobBannerAd();
    // testAdMobNativeAd();
    // testAdMobInterstitial();
    // testAdMobRewardedAd();
    // testAppLovin();
    // testUnityAdsRewardedAd();
    // testIronSourceRewardedAd();
    // testVungle();
    // testMultiAds();
    // testFacebookInterstitialAd();
    // testFacebookNativeAd();

    cocos2d::log("Create scene");
    // director->runWithScene(VideoPlayerTestScene::create());
    // director->runWithScene(createMultiNativeAdTestScene());

    // Deprecated.
    // director->runWithScene(TwitterShareTestScene::create());

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    cocos2d::log(__PRETTY_FUNCTION__);
    cocos2d::Director::getInstance()->stopAnimation();
#ifndef EE_X_DESKTOP
    // FIXME.
    // NotificationAgent::getInstance()->scheduleAll();
#endif // EE_X_DESKTOP
}

void AppDelegate::applicationWillEnterForeground() {
    cocos2d::log(__PRETTY_FUNCTION__);
    cocos2d::Director::getInstance()->startAnimation();
#ifndef EE_X_DESKTOP
    // FIXME.
    // NotificationAgent::getInstance()->unscheduleAll();
#endif // EE_X_DESKTOP
}
} // namespace eetest
