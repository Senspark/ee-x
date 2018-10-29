//
//  EETransitionDialog.cpp
//  ee-library
//
//  Created by enrevol on 12/1/15.
//
//

#include "EETransitionFadeOut.hpp"

#include <cocos2d.h>

namespace ee {
TransitionFadeOut* TransitionFadeOut::create(float duration,
                                             cocos2d::Scene* scene,
                                             const cocos2d::Color3B& color) {
    auto result = new (std::nothrow) TransitionFadeOut();
    result->initWithDuration(duration, scene, color);
    result->autorelease();
    return result;
}

TransitionFadeOut* TransitionFadeOut::create(float duration,
                                             cocos2d::Scene* scene) {
    return TransitionFadeOut::create(duration, scene, cocos2d::Color3B::BLACK);
}

bool TransitionFadeOut::initWithDuration(float duration, cocos2d::Scene* scene,
                                         const cocos2d::Color3B& color) {
    if (Super::initWithDuration(duration, scene)) {
        color_.r = color.r;
        color_.g = color.g;
        color_.b = color.b;
        color_.a = 0;
    }
    return true;
}

bool TransitionFadeOut::initWithDuration(float t, cocos2d::Scene* scene) {
    return initWithDuration(t, scene, cocos2d::Color3B::BLACK);
}

void TransitionFadeOut::onEnter() {
    Super::onEnter();

    _inScene->setVisible(false);

    layerColor_ = cocos2d::LayerColor::create(color_);
    addChild(layerColor_, 2);
    layerColor_->runAction(cocos2d::Sequence::create(
        cocos2d::FadeIn::create(_duration), cocos2d::CallFunc::create([this] {
            hideOutShowIn();
            finish();
        }),
        nullptr));
}

void TransitionFadeOut::onExit() {
    Super::onExit();
    layerColor_->removeFromParentAndCleanup(false);
}
} // namespace ee
