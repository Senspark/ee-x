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

    adsManager_->setBannerAdVisible(true);
    schedule(std::bind([this, winSize] {
                 CCLOG("Move to top-left");
                 adsManager_->setBannerAdAnchor(0, 1);
                 adsManager_->setBannerAdPosition(0, winSize.height);
             }),
             8.0f, CC_REPEAT_FOREVER, 2.0f, "_1");
    schedule(std::bind([this, winSize] {
                 CCLOG("Move to top-right");
                 adsManager_->setBannerAdAnchor(1, 1);
                 adsManager_->setBannerAdPosition(winSize.width,
                                                  winSize.height);
             }),
             8.0f, CC_REPEAT_FOREVER, 4.0f, "_2");
    schedule(std::bind([this, winSize] {
                 CCLOG("Move to bottom-right");
                 adsManager_->setBannerAdAnchor(1, 0);
                 adsManager_->setBannerAdPosition(winSize.width, 0);
             }),
             8.0f, CC_REPEAT_FOREVER, 6.0f, "_3");
    schedule(std::bind([this, winSize] {
                 CCLOG("Move to bottom-left");
                 adsManager_->setBannerAdAnchor(0, 0);
                 adsManager_->setBannerAdPosition(0, 0);
             }),
             8.0f, CC_REPEAT_FOREVER, 8.0f, "_4");
    return true;
}
} // namespace eetest
