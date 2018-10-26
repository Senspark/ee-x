//
//  EEConsole.cpp
//  ee-library
//
//  Created by Zinge on 4/10/17.
//
//

#include "EEConsole.hpp"
#include "EEConsoleDelegate.hpp"
#include "EECommandPool.hpp"
#include "EEDialogComponent.hpp"

#include <2d/CCLayer.h>
#include <2d/CCSprite.h>
#include <base/CCDirector.h>
#include <ui/UIEditBox/UIEditBox.h>

namespace ee {
Console* Console::create() {
    auto result = new Self();
    result->init();
    result->autorelease();
    return result;
}

Console::Console()
    : editBox_(nullptr)
    , editBoxDelegate_(nullptr) {
}

Console::~Console() = default;

bool Console::init() {
    if (not Super::init()) {
        return false;
    }
    auto&& winSize = _director->getWinSize();

    setTouchEnabled(true);
    setSwallowTouches(true);
    setContentSize(cocos2d::Size(winSize.width, 50.0f));

    editBoxDelegate_ = std::make_unique<detail::ConsoleDelegate>(this);

    auto whiteSprite = cocos2d::Sprite::create();
    auto editBoxSprite = cocos2d::ui::Scale9Sprite::createWithSpriteFrame(
        whiteSprite->getSpriteFrame());
    editBoxSprite->setVisible(false);

    auto background = cocos2d::LayerColor::create(cocos2d::Color4B::BLACK);
    background->setContentSize(getContentSize());
    background->setOpacity(100);
    addChild(background);

    editBox_ = cocos2d::ui::EditBox::create(cocos2d::Size(winSize.width, 50.0f),
                                            editBoxSprite);
    editBox_->setSwallowTouches(false);
    editBox_->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_TOP);
    editBox_->setPositionPercent(cocos2d::Vec2::ANCHOR_MIDDLE_TOP);
    editBox_->setPositionType(cocos2d::ui::Widget::PositionType::PERCENT);
    editBox_->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    editBox_->setInputFlag(
        cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);
    editBox_->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::SEND);
    editBox_->setFontSize(20);
    editBox_->setFontName("Arial");
    editBox_->setFontColor(cocos2d::Color4B(200, 200, 200, 255));
    editBox_->setDelegate(editBoxDelegate_.get());
    editBox_->addComponent(ee::DialogComponent::create());
    addChild(editBox_);

    addComponent(ee::DialogComponent::create());
    return true;
}

bool Console::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    editBox_->addTouchEventListener(
        std::bind(&cocos2d::ui::EditBox::touchDownAction, editBox_,
                  std::placeholders::_1, std::placeholders::_2));
    return Super::onTouchBegan(touch, event);
}

void Console::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {
    Super::onTouchMoved(touch, event);
    setPosition(getPosition() + touch->getDelta());
    if (touch->getDelta().getLengthSq() >= 10.0f) {
        // Disable editing.
        editBox_->addTouchEventListener(nullptr);
    }
}

void Console::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
    Super::onTouchEnded(touch, event);
}

Console*
Console::addCommandPool(std::unique_ptr<detail::CommandPoolBase> pool) {
    pools_.push_back(std::move(pool));
    return this;
}
} // namespace ee
