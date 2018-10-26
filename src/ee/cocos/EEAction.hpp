//
//  EEAction.hpp
//  SDE-PRO
//
//  Created by Zinge on 3/24/17.
//
//

#ifndef EE_LIBRARY_ACTION_HPP_
#define EE_LIBRARY_ACTION_HPP_

#include <2d/CCActionInterval.h>

namespace ee {
class Sequence : public cocos2d::Sequence {
private:
    using Self = Sequence;
    using Super = cocos2d::Sequence;

public:
    static Self* create();

    Self* delay(float duration);

    Self* then(const std::function<void()>& callback);

    Self* then(cocos2d::FiniteTimeAction* action);

private:
    Sequence();

    std::size_t count_;
};

class Spawn : public cocos2d::Spawn {
private:
    using Self = Spawn;
    using Super = cocos2d::Spawn;

public:
    static Self* create();

    Self* delay(float duration);

    Self* with(const std::function<void()>& callback);

    Self* with(cocos2d::FiniteTimeAction* action);

private:
    Spawn();

    std::size_t count_;
};

class RelativeMoveBy : public cocos2d::ActionInterval {
private:
    using Self = RelativeMoveBy;
    using Super = cocos2d::ActionInterval;

public:
    static Self* create(float duration, const cocos2d::Vec2& delta);

    virtual Self* clone() const override;
    virtual Self* reverse() const override;

    virtual void startWithTarget(cocos2d::Node* target) override;

    virtual void update(float time) override;

protected:
    bool initWithDuration(float duration, const cocos2d::Vec2& deltaPosition);

    cocos2d::Vec2 deltaPosition_;
    cocos2d::Vec2 startPosition_;
    cocos2d::Vec2 previousPosition_;
};

class RelativeMoveTo : public RelativeMoveBy {
private:
    using Self = RelativeMoveTo;
    using Super = RelativeMoveBy;

public:
    static Self* create(float duration, const cocos2d::Vec2& position);

    virtual Self* clone() const override;
    virtual Self* reverse() const override;

    virtual void startWithTarget(cocos2d::Node* target) override;

protected:
    bool initWithDuration(float duration, const cocos2d::Vec2& endPosition);

    cocos2d::Vec2 endPosition_;
};
} // namespace ee

#endif /* EE_LIBRARY_ACTION_HPP_ */
