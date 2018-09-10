//
//  EETransitionDialog.hpp
//  ee-library
//
//  Created by enrevol on 12/1/15.
//
//

#ifndef EE_LIBRARY_TRANSITION_FADE_OUT_HPP_
#define EE_LIBRARY_TRANSITION_FADE_OUT_HPP_

#include "EECocos2dxFwd.hpp"

#include <2d/CCTransition.h>

namespace ee {
class TransitionFadeOut : public cocos2d::TransitionScene {
private:
    using Super = cocos2d::TransitionScene;

public:
    static TransitionFadeOut* create(float duration, cocos2d::Scene* scene,
                                     const cocos2d::Color3B& color);

    static TransitionFadeOut* create(float duration, cocos2d::Scene* scene);

    virtual void onEnter() override;

    virtual void onExit() override;

protected:
    TransitionFadeOut() = default;
    virtual ~TransitionFadeOut() override = default;

    bool initWithDuration(float duration, cocos2d::Scene* scene,
                          const cocos2d::Color3B& color);

    bool initWithDuration(float duration, cocos2d::Scene* scene);

private:
    cocos2d::LayerColor* layerColor_;
    cocos2d::Color4B color_;
};
} // namespace ee

#endif /* EE_LIBRARY_TRANSITION_FADE_OUT_HPP_ */
