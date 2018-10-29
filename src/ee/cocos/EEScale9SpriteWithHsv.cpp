//
//  EEScale9SpriteWithHSV.cpp
//  ee-library
//
//  Created by Zinge on 10/24/16.
//
//

#include "EEScale9SpriteWithHsv.hpp"
#include "EEShaderUtils.hpp"
#include "EEUtils.hpp"

#include <cocos2d.h> // For COCOS2D_VERSION

namespace ee {
using Self = Scale9SpriteWithHsv;

Self* Self::create() {
    return createInstance<Self>(
        [](Self* instance) { return instance->init(); });
}

Self* Self::create(const std::string& filename) {
    return createInstance<Self>(
        [&](Self* instance) { return instance->initWithFile(filename); });
}

Self* Self::create(const std::string& filename, const cocos2d::Rect& rect) {
    return createInstance<Self>(
        [&](Self* instance) { return instance->initWithFile(filename, rect); });
}

Self* Self::createWithSpriteFrame(cocos2d::SpriteFrame* spriteFrame) {
    return createInstance<Self>([&](Self* instance) {
        return instance->initWithSpriteFrame(spriteFrame);
    });
}

Self* Self::createWithSpriteFrameName(const std::string& spriteFrameName) {
    return createInstance<Self>([&](Self* instance) {
        return instance->initWithSpriteFrameName(spriteFrameName);
    });
}

Self::Scale9SpriteWithHsv()
    : customState_(nullptr) {}

bool Self::init() {
    if (not Super::init()) {
        return false;
    }
#if COCOS2D_VERSION >= 0x00031400
    customState_ = createState();
    setGLProgramState(customState_);
#endif // COCOS2D_VERSION >= 0x00031400
    return true;
}

void Self::setGLProgramState(cocos2d::GLProgramState* glProgramState) {
#if COCOS2D_VERSION >= 0x00031400
    // cocos2d::ui::Scale9Sprite::setState is not virtual.
    if (glProgramState != customState_) {
        return;
    }
#endif // COCOS2D_VERSION >= 0x00031400
    Super::setGLProgramState(glProgramState);
}

bool Self::updateWithSprite(cocos2d::Sprite* sprite,
                            const cocos2d::Rect& textureRect, bool rotated,
                            const cocos2d::Vec2& offset,
                            const cocos2d::Size& originalSize,
                            const cocos2d::Rect& capInsets) {
    if (not Super::updateWithSprite(sprite, textureRect, rotated, offset,
                                    originalSize, capInsets)) {
        return false;
    }
#if COCOS2D_VERSION < 0x00031400
    _scale9Image->setGLProgramState(createState());
#endif // COCOS2D_VERSION < 0x00031400
    return true;
}

void Self::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform,
                std::uint32_t flags) {
    updateMatrix();
    Super::draw(renderer, transform, flags);
}

cocos2d::GLProgramState* Self::createState() {
    auto state = createHsvProgramState();
    state->setUniformCallback("u_hsv", [this](cocos2d::GLProgram* p,
                                              cocos2d::Uniform* u) {
        p->setUniformLocationWithMatrix4fv(u->location, getHsvMatrix().m, 1);
    });
    return state;
}
} // namespace ee
