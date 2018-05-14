//
//  NativeAdTestScene.cpp
//  ee_x_test
//
//  Created by Zinge on 11/6/17.
//
//

#include "NativeAdTestScene.hpp"

#include <ee/Ads.hpp>
#include <ee/Core.hpp>

#include <cocos2d.h>
#include <ui/CocosGUI.h>

namespace eetest {
using Self = NativeAdTestScene;

namespace {
cocos2d::ui::Button* createButton() {
    auto button = cocos2d::ui::Button::create("images/button.png");
    button->setTitleFontSize(50);
    button->setScale(0.25f);
    return button;
}
} // namespace

Self* Self::createWithAdView(std::shared_ptr<ee::IAdView> adView) {
    auto result = new Self();
    result->initWithAdView(adView);
    result->autorelease();
    return result;
}

bool Self::initWithAdView(std::shared_ptr<ee::IAdView> adView) {
    if (not Super::init()) {
        return false;
    }
    auto background =
        cocos2d::LayerColor::create(cocos2d::Color4B(127, 127, 127, 127));
    addChild(background);

    auto&& frameSize =
        cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
    int screenWidth = static_cast<int>(frameSize.width);
    int screenHeight = static_cast<int>(frameSize.height);

    adView_ = adView;

    auto loadButton = createButton();
    loadButton->setNormalizedPosition(cocos2d::Vec2(0.4f, 0.6f));
    loadButton->setTitleText("LOAD");
    loadButton->addClickEventListener(
        std::bind([this] { ee::runOnUiThread([this] { adView_->load(); }); }));
    addChild(loadButton);

    auto resizeButton = createButton();
    resizeButton->setNormalizedPosition(cocos2d::Vec2(0.6f, 0.6f));
    resizeButton->setTitleText("RESIZE");
    resizeButton->addTouchEventListener(std::bind(
        [this, resizeButton](cocos2d::ui::Widget::TouchEventType type) {
            if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
                firstTouch_ = true;
                return;
            }
            if (type == cocos2d::ui::Widget::TouchEventType::MOVED) {
                if (not firstTouch_) {
                    auto delta =
                        resizeButton->getTouchMovePosition() - lastPosition_;
                    auto deltaXInPixels =
                        ee::Metrics::fromPoint(delta.x).toPixel();
                    auto deltaYInPixels =
                        ee::Metrics::fromPoint(delta.y).toPixel();
                    ee::runOnUiThread([this, deltaXInPixels, deltaYInPixels] {
                        int width, height;
                        std::tie(width, height) = adView_->getSize();
                        adView_->setSize(
                            width + static_cast<int>(deltaXInPixels),
                            height - static_cast<int>(deltaYInPixels));
                    });
                }
                firstTouch_ = false;
                lastPosition_ = resizeButton->getTouchMovePosition();
            }
        },
        std::placeholders::_2));
    addChild(resizeButton);

    auto showButton = createButton();
    showButton->setNormalizedPosition(cocos2d::Vec2(0.4f, 0.4f));
    showButton->setTitleText("SHOW");
    showButton->addClickEventListener(std::bind(
        [this] { ee::runOnUiThread([this] { adView_->setVisible(true); }); }));
    addChild(showButton);

    auto hideButton = createButton();
    hideButton->setNormalizedPosition(cocos2d::Vec2(0.6f, 0.4f));
    hideButton->setTitleText("HIDE");
    hideButton->addClickEventListener(std::bind(
        [this] { ee::runOnUiThread([this] { adView_->setVisible(false); }); }));
    addChild(hideButton);

    auto moveTopLeftButton = createButton();
    moveTopLeftButton->setNormalizedPosition(cocos2d::Vec2(0.1f, 0.9f));
    moveTopLeftButton->setTitleText("MOVE");
    moveTopLeftButton->addClickEventListener(std::bind([this] {
        ee::runOnUiThread([this] {
            adView_->setAnchor(0, 0);
            adView_->setPosition(0, 0);
        });
    }));
    addChild(moveTopLeftButton);

    auto moveTopRightButton = createButton();
    moveTopRightButton->setNormalizedPosition(cocos2d::Vec2(0.9f, 0.9f));
    moveTopRightButton->setTitleText("MOVE");
    moveTopRightButton->addClickEventListener(std::bind([this, screenWidth] {
        ee::runOnUiThread([this, screenWidth] {
            adView_->setAnchor(1, 0);
            adView_->setPosition(screenWidth, 0);
        });
    }));
    addChild(moveTopRightButton);

    auto moveBottomLeftButton = createButton();
    moveBottomLeftButton->setNormalizedPosition(cocos2d::Vec2(0.1f, 0.1f));
    moveBottomLeftButton->setTitleText("MOVE");
    moveBottomLeftButton->addClickEventListener(std::bind([this, screenHeight] {
        ee::runOnUiThread([this, screenHeight] {
            adView_->setAnchor(0, 1);
            adView_->setPosition(0, screenHeight);
        });
    }));
    addChild(moveBottomLeftButton);

    auto moveBottomRightButton = createButton();
    moveBottomRightButton->setNormalizedPosition(cocos2d::Vec2(0.9f, 0.1f));
    moveBottomRightButton->setTitleText("MOVE");
    moveBottomRightButton->addClickEventListener(
        std::bind([this, screenWidth, screenHeight] {
            ee::runOnUiThread([this, screenWidth, screenHeight] {
                adView_->setAnchor(1, 1);
                adView_->setPosition(screenWidth, screenHeight);
            });
        }));
    addChild(moveBottomRightButton);

    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan = std::bind([] { return true; });
    listener->onTouchMoved = std::bind(
        [this](cocos2d::Touch* touch) {
            auto deltaX = touch->getDelta().x;
            auto deltaY = touch->getDelta().y;
            auto deltaXInPixels = ee::Metrics::fromPoint(deltaX).toPixel();
            auto deltaYInPixels = ee::Metrics::fromPoint(deltaY).toPixel();
            ee::runOnUiThread([this, deltaXInPixels, deltaYInPixels] {
                int x, y;
                std::tie(x, y) = adView_->getPosition();
                adView_->setPosition(x + static_cast<int>(deltaXInPixels),
                                     y - static_cast<int>(deltaYInPixels));
            });
        },
        std::placeholders::_1);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,
                                                                 this);
    return true;
}

void Self::onEnter() {
    Super::onEnter();
}

void Self::onExit() {
    Super::onExit();
}
} // namespace eetest
