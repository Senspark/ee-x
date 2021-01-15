//
//  AppDelegate.cpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#include "AppDelegate.hpp"

#include <cocos2d.h>

#include <ee/Cpp.hpp>

#include "CoreTester.hpp"
#include "Utils.hpp"
#include "VideoPlayerTestScene.hpp"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID ||                               \
    CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "FirebaseCrashlyticsTester.hpp"
#include "RewardedAdSceneTester.hpp"
#endif

namespace eetest {
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
    getLogger().info("frameSize = %f %f", frameSize.width, frameSize.height);

    auto&& winSize = director->getWinSize();
    getLogger().info("winSize = %f %f", winSize.width, winSize.height);

    ee::PluginManager::initializePlugins();

    constexpr float points = 1;
    auto metrics = ee::Metrics::fromPoint(points);
    auto dp = metrics.toDip();
    auto pixels = metrics.toPixel();
    getLogger().info("%f pt = %f pixels = %f dp", points, pixels, dp);

    auto [viewWidth, viewHeight] = ee::getViewSize();
    auto [screenWidth, screenHeight] = ee::getScreenSize();
    getLogger().info("View size = %d %d", viewWidth, viewHeight);
    getLogger().info("Screen size = %d %d", screenWidth, screenHeight);

    ee::Logger::setSystemLogger(getLogger());

    static std::vector<std::shared_ptr<ITester>> testers;
    testers.push_back(std::make_shared<CoreTester>());
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID ||                               \
    CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    testers.push_back(std::make_shared<FirebaseCrashlyticsTester>());
    testers.push_back(std::make_shared<RewardedAdSceneTester>());
#endif
    for (auto&& tester : testers) {
        tester->initialize();
        tester->start();
    }
    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    cocos2d::log(__PRETTY_FUNCTION__);
    cocos2d::Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
    cocos2d::log(__PRETTY_FUNCTION__);
    cocos2d::Director::getInstance()->startAnimation();
}
} // namespace eetest
