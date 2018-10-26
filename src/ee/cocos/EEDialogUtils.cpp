//
//  EEDialogUtils.cpp
//  ee-library
//
//  Created by Zinge on 5/12/16.
//
//

#include "EEDialogUtils.hpp"
#include "EEDialog.hpp"
#include "EEUtils.hpp"

#include <2d/CCActionEase.h>
#include <2d/CCActionInstant.h>
#include <2d/CCActionInterval.h>
#include <2d/CCSprite.h>
#include <base/CCDirector.h>

NS_EE_BEGIN
Dialog* addHorizontalTransition(Dialog* dialog, float duration, float rate) {
    auto&& winSize = cocos2d::Director::getInstance()->getWinSize();

    dialog->addDialogWillShowCallback([&winSize](Dialog* dlg) {
        dlg->setPosition(
            cocos2d::Point(-winSize.width / 2, winSize.height / 2));
    });

    auto move =
        cocos2d::MoveBy::create(duration, cocos2d::Vec2(winSize.width, 0));
    auto moveIn = cocos2d::EaseElasticOut::create(move->clone(), rate);
    auto moveOut = cocos2d::EaseElasticIn::create(move->clone(), rate);

    dialog->addShowingTransition(moveIn);
    dialog->addHidingTransition(moveOut);

    return dialog;
}

Dialog* addBlurBackground(Dialog* dialog, float duration, float scale,
                          int blurRadius) {
    constexpr const char* BackgroundSpriteName = "blurred-background-sprite";

    dialog->addDialogWillShowCallback([=](Dialog* dlg) {
        auto sprite = captureBlurredScreenInPoints(scale, blurRadius);
        sprite->setScale(1 / scale);
        sprite->setNormalizedPosition(cocos2d::Vec2::ANCHOR_MIDDLE);
        sprite->setName(BackgroundSpriteName);

        sprite->setOpacity(0);
        sprite->runAction(cocos2d::FadeIn::create(duration));

        dlg->getContainer()->addChild(sprite,
                                      std::numeric_limits<int>::lowest());
    });

    dialog->addDialogWillHideCallback([=](Dialog* dlg) {
        auto sprite = dlg->getContainer()->getChildByName(BackgroundSpriteName);
        CC_ASSERT(sprite != nullptr);

        auto sequence =
            cocos2d::Sequence::create(cocos2d::FadeOut::create(duration),
                                      cocos2d::RemoveSelf::create(), nullptr);

        sprite->runAction(sequence);
    });

    return dialog;
}
NS_EE_END
