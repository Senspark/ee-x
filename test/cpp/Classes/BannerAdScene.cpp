//
//  TestBannerAdScene.cpp
//  ee_x_test
//
//  Created by eps on 1/25/21.
//

#include "BannerAdScene.hpp"

#include <cocos2d.h>

#include <ee/Cpp.hpp>

namespace eetest {
using Self = BannerAdScene;

cocos2d::Scene*
Self::openScene(const std::shared_ptr<ee::IAdsManager>& adsManager) {
    auto scene = cocos2d::Scene::create();
    auto node = new Self();
    node->autorelease();
    node->adsManager_ = adsManager;
    node->init();
    scene->addChild(node);
    return scene;
}

bool Self::init() {
    if (not Super::init()) {
        return false;
    }
    auto&& winSize = _director->getWinSize();
    setContentSize(winSize);

    auto&& ad = adsManager_->getBannerAd();
    ad->setVisible(true);
    schedule(std::bind([winSize, ad] {
                 CCLOG("Move to top-left");
                 ad->setAnchor(0, 1);
                 ad->setPosition(0, winSize.height);
             }),
             8.0f, CC_REPEAT_FOREVER, 2.0f, "_1");
    schedule(std::bind([winSize, ad] {
                 CCLOG("Move to top-right");
                 ad->setAnchor(1, 1);
                 ad->setPosition(winSize.width, winSize.height);
             }),
             8.0f, CC_REPEAT_FOREVER, 4.0f, "_2");
    schedule(std::bind([winSize, ad] {
                 CCLOG("Move to bottom-right");
                 ad->setAnchor(1, 0);
                 ad->setPosition(winSize.width, 0);
             }),
             8.0f, CC_REPEAT_FOREVER, 6.0f, "_3");
    schedule(std::bind([winSize, ad] {
                 CCLOG("Move to bottom-left");
                 ad->setAnchor(0, 0);
                 ad->setPosition(0, 0);
             }),
             8.0f, CC_REPEAT_FOREVER, 8.0f, "_4");
    return true;
}
} // namespace eetest
