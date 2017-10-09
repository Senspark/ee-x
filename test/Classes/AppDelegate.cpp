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

#include <ee/Core.hpp>
#include <ee/FacebookAds.hpp>
#include <ee/Macro.hpp>

namespace {
const auto DesignResolution = cocos2d::Size(480, 320);
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
    cocos2d::log("frameSize = %f %f", frameSize.width, frameSize.height);

    auto&& winSize = director->getWinSize();
    cocos2d::log("winSize = %f %f", winSize.width, winSize.height);

    ee::Metrics::initialize(frameSize.height / winSize.height);
    constexpr float points = 200;
    auto metrics = ee::Metrics::fromPoint(points);
    auto dp = metrics.toDip();
    auto pixels = metrics.toPixel();
    cocos2d::log("%f pt = %f pixels = %f dp", points, pixels, dp);

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

    cocos2d::log("Create FacebookAds plugin");
    static auto plugin = ee::FacebookAds();

    ee::runOnUiThread([] {
        cocos2d::log("Create Facebook native ad begin");
        static auto native = plugin.createNativeAd(
            ee::FacebookNativeAdBuilder()
                .setAdId("869337403086643_1444948412192203")
                .setLayoutName("fb_native_spin"));
        // native->setVisible(true);
        // native->setPosition(400, 100);
        cocos2d::log("Create Facebook native ad end");

        cocos2d::log("Native ad size: %d %d", native->getSize().first,
                     native->getSize().second);
        cocos2d::log("Native ad position: %d %d", native->getPosition().first,
                     native->getPosition().second);
    });

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
