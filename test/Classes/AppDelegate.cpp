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

namespace {
ee::Logger& getLogger() {
    static ee::Logger logger("ee_x");
    return logger;
}

void scheduleOnce(float delay, const std::function<void()>& f) {
    static int target;
    static int counter;
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->schedule(std::bind(f), &target, 0, 0, delay, false,
                        std::to_string(counter++));
}

ee::FacebookAds* facebookAds_;
std::shared_ptr<ee::FacebookNativeAd> facebookNativeAd_;
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

    int screenWidth = static_cast<int>(frameSize.width);
    int screenHeight = static_cast<int>(frameSize.height);

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

    float delay = 0.0f;
    scheduleOnce(delay += 1.0f, [] {
        getLogger().info("Create FacebookAds plugin");
        static auto plugin = ee::FacebookAds();
        facebookAds_ = &plugin;
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
        });
    });

    scheduleOnce(delay += 1.0f, [screenWidth, screenHeight] {
        ee::runOnUiThread([screenWidth, screenHeight] {
            getLogger().info("Resize = screen size / 2");
            facebookNativeAd_->setPosition(screenWidth / 4, screenHeight / 4);
            facebookNativeAd_->setSize(screenWidth / 2, screenHeight / 2);
        });
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
