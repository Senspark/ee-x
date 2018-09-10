//
//  EEAction.cpp
//  SDE-PRO
//
//  Created by Zinge on 3/24/17.
//
//

#include "EEAction.hpp"
#include "EEUtils.hpp"

#include <2d/CCActionInstant.h>
#include <base/CCRefPtr.h>

namespace ee {
namespace {
class ExtraAction : public cocos2d::FiniteTimeAction {
public:
    static ExtraAction* create();
    virtual ExtraAction* clone() const override;
    virtual ExtraAction* reverse(void) const override;
    virtual void update(float time) override;
    virtual void step(float delta) override;
};

ExtraAction* ExtraAction::create() {
    auto result = new ExtraAction();
    result->autorelease();
    return result;
}
ExtraAction* ExtraAction::clone() const {
    return create();
}

ExtraAction* ExtraAction::reverse() const {
    return create();
}

void ExtraAction::update(float time) {
    CC_UNUSED_PARAM(time);
}

void ExtraAction::step(float delta) {
    CC_UNUSED_PARAM(delta);
}
} // namespace

Sequence* Sequence::create() {
    auto result = new Sequence();
    result->autorelease();
    result->initWithTwoActions(ExtraAction::create(), ExtraAction::create());
    return result;
}

Sequence::Sequence()
    : count_(0) {
}

Sequence* Sequence::then(cocos2d::FiniteTimeAction* action) {
    auto guard0 = ee::make_ref_guard(_actions[0]);
    auto guard1 = ee::make_ref_guard(_actions[1]);
    _actions[0]->release();
    _actions[1]->release();
    if (count_ == 0) {
        initWithTwoActions(action, _actions[1]);
    } else if (count_ == 1) {
        initWithTwoActions(_actions[0], action);
    } else {
        initWithTwoActions(
            Sequence::create()->then(_actions[0])->then(_actions[1]), action);
    }
    ++count_;
    return this;
}

Sequence* Sequence::delay(float duration) {
    return then(cocos2d::DelayTime::create(duration));
}

Sequence* Sequence::then(const std::function<void()>& callback) {
    return then(cocos2d::CallFunc::create(callback));
}

Spawn* Spawn::create() {
    auto result = new Spawn();
    result->autorelease();
    result->initWithTwoActions(ExtraAction::create(), ExtraAction::create());
    return result;
}

Spawn::Spawn()
    : count_(0) {
}

Spawn* Spawn::with(cocos2d::FiniteTimeAction* action) {
    auto guard0 = ee::make_ref_guard(_one);
    auto guard1 = ee::make_ref_guard(_two);
    _one->release();
    _two->release();
    if (count_ == 0) {
        initWithTwoActions(action, _two);
    } else if (count_ == 1) {
        initWithTwoActions(_one, action);
    } else {
        initWithTwoActions(Spawn::create()->with(_one)->with(_two), action);
    }
    ++count_;
    return this;
}

Spawn* Spawn::delay(float duration) {
    return with(cocos2d::DelayTime::create(duration));
}

Spawn* Spawn::with(const std::function<void()>& callback) {
    return with(cocos2d::CallFunc::create(callback));
}

RelativeMoveBy* RelativeMoveBy::create(float duration,
                                       const cocos2d::Vec2& delta) {
    auto result = new RelativeMoveBy();
    result->initWithDuration(duration, delta);
    result->autorelease();
    return result;
}

bool RelativeMoveBy::initWithDuration(float duration,
                                      const cocos2d::Vec2& deltaPosition) {
    if (not Super::initWithDuration(duration)) {
        return false;
    }
    deltaPosition_ = deltaPosition;
    return true;
}

RelativeMoveBy* RelativeMoveBy::clone() const {
    return RelativeMoveBy::create(_duration, deltaPosition_);
}

RelativeMoveBy* RelativeMoveBy::reverse() const {
    return RelativeMoveBy::create(_duration, -deltaPosition_);
}

void RelativeMoveBy::startWithTarget(cocos2d::Node* target) {
    Super::startWithTarget(target);
    previousPosition_ = startPosition_ = target->getNormalizedPosition();
}

void RelativeMoveBy::update(float time) {
    if (_target == nullptr) {
        return;
    }

    auto currentPosition = _target->getNormalizedPosition();
    auto diff = currentPosition - previousPosition_;
    startPosition_ += diff;
    auto newPosition = startPosition_ + deltaPosition_ * time;

    // Force to use normalized position.
    const auto AlmostZero = cocos2d::Vec2(0.0001f, 0.0001f);
    _target->setNormalizedPosition(AlmostZero);

    _target->setNormalizedPosition(newPosition);
    previousPosition_ = newPosition;
}

RelativeMoveTo* RelativeMoveTo::create(float duration,
                                       const cocos2d::Vec2& position) {
    auto result = new RelativeMoveTo();
    result->initWithDuration(duration, position);
    result->autorelease();
    return result;
}

bool RelativeMoveTo::initWithDuration(float duration,
                                      const cocos2d::Vec2& endPosition) {
    if (not ActionInterval::initWithDuration(duration)) {
        return false;
    }
    endPosition_ = endPosition;
    return true;
}

RelativeMoveTo* RelativeMoveTo::clone() const {
    return RelativeMoveTo::create(_duration, endPosition_);
}

RelativeMoveTo* RelativeMoveTo::reverse() const {
    CC_ASSERT(false);
    return nullptr;
}

void RelativeMoveTo::startWithTarget(cocos2d::Node* target) {
    Super::startWithTarget(target);
    deltaPosition_ = endPosition_ - target->getNormalizedPosition();
}
} // namespace ee
