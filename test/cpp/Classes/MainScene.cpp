//
//  TestScene.cpp
//  ee_x_test
//
//  Created by eps on 1/25/21.
//

#include "MainScene.hpp"

#include <cocos2d.h>
#include <ui/CocosGUI.h>

#include <ee/Cpp.hpp>

#include "BannerAdScene.hpp"
#include "FullScreenAdScene.hpp"

namespace eetest {
using Self = MainScene;

cocos2d::Scene* Self::openScene() {
    auto scene = cocos2d::Scene::create();
    auto node = new Self();
    node->autorelease();
    node->init();
    scene->addChild(node);
    return scene;
}

bool Self::init() {
    if (not Super::init()) {
        return false;
    }
    auto fileUtils = cocos2d::FileUtils::getInstance();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    auto&& fileName = "ads_config_ios.json";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    auto&& fileName = "config/ads_config_android.json";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    auto adsConfig = fileUtils->getStringFromFile(fileName);
    auto adsManager = std::make_shared<ee::DefaultAdsManager>(adsConfig);
    ee::noAwait(adsManager->initialize());

    auto&& winSize = _director->getWinSize();
    setContentSize(winSize);

    auto testBannerAdButton = cocos2d::ui::Button::create();
    testBannerAdButton->setContentSize(cocos2d::Size(200, 60));
    testBannerAdButton->setPosition(
        cocos2d::Point(winSize.width / 2, winSize.height / 2 + 25));
    testBannerAdButton->setTitleText("Test Banner Ad");
    testBannerAdButton->addClickEventListener(std::bind([this, adsManager] {
        auto scene = BannerAdScene::openScene(adsManager);
        _director->replaceScene(scene);
    }));

    auto testFullScreenAdButton = cocos2d::ui::Button::create();
    testFullScreenAdButton->setContentSize(cocos2d::Size(200, 60));
    testFullScreenAdButton->setPosition(
        cocos2d::Point(winSize.width / 2, winSize.height / 2 - 25));
    testFullScreenAdButton->setTitleText("Test Full-screen Ad");
    testFullScreenAdButton->addClickEventListener(std::bind([this, adsManager] {
        auto scene = FullScreenAdScene::openScene(adsManager);
        _director->replaceScene(scene);
    }));

    addChild(testBannerAdButton);
    addChild(testFullScreenAdButton);
    return true;
}
} // namespace eetest
