//
//  TwitterShareTestScene.cpp
//  ee_x_test_mobile
//
//  Created by Doan Anh Tai on 10/11/18.
//

#include "TwitterShareTestScene.hpp"

#include <ee/Core.hpp>
#include <ee/Twitter.hpp>

#include <cocos2d.h>
#include <ui/CocosGUI.h>

namespace {
cocos2d::ui::Button* createButton() {
    auto button = cocos2d::ui::Button::create("images/button.png");
    button->setTitleFontSize(50);
    button->setScale(0.25f);
    return button;
}
} // namespace

namespace eetest {
using Self = TwitterShareTestScene;

Self* Self::create() {
    auto result = new Self();
    result->init();
    result->autorelease();
    return result;
}

Self::TwitterShareTestScene() {
    twitter_ = std::make_unique<ee::Twitter>();
    twitter_->initialize("ConsumerKey", "ConsumerSecret");
    twitter_->setShareResultCallback(
        [this](bool value) { this->doShareSuccess(value); });
}

bool Self::init() {
    if (not Super::init()) {
        return false;
    }

    auto screenSize = cocos2d::Director::getInstance()->getWinSize();

    auto shareButton = createButton();
    shareButton->setPosition(
        cocos2d::Vec2(screenSize.width / 2, screenSize.height / 2));
    shareButton->setTitleText("Twitter");
    shareButton->addClickEventListener(std::bind([this] {
        ee::runOnUiThread([this] {
            twitter_->shareContent("this is a string to twitter...");
        });
    }));
    addChild(shareButton);

    auto lb = cocos2d::Label::create();
    lb->setString("press twitter to twitter...");
    lb->setPosition(
        cocos2d::Vec2(screenSize.width / 2, screenSize.height / 2 - 30));
    lb->setName("label");
    addChild(lb);

    return true;
}

void Self::onEnter() {
    Super::onEnter();
}

void Self::onExit() {
    Super::onExit();
}

void Self::doShareSuccess(bool result) {
    auto lb = dynamic_cast<cocos2d::Label*>(getChildByName("label"));
    if (result) {
        lb->setString("Twitter Success");
    } else {
        lb->setString("Twitter Fail");
    }
}

} // namespace eetest
