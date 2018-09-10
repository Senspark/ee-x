//
//  EESpriteWithHSV.cpp
//  ee-library
//
//  Created by Zinge on 6/22/16.
//
//

#include "EESpriteWithHsv.hpp"
#include "EEShaderUtils.hpp"
#include "EEUtils.hpp"

#include <cocos2d.h>
#include <renderer/CCGLProgramState.h>

namespace ee {
using Self = SpriteWithHsv;

Self* Self::create() {
    return createInstance<Self>(std::bind(&Self::init, std::placeholders::_1));
}

Self* Self::create(const std::string& filename) {
    return createInstance<Self>(
        [&](Self* instance) { return instance->initWithFile(filename); });
}

Self* Self::create(const std::string& filename, const cocos2d::Rect& rect) {
    return createInstance<Self>(
        [&](Self* instance) { return instance->initWithFile(filename, rect); });
}

Self* Self::createWithTexture(cocos2d::Texture2D* texture) {
    return createInstance<Self>(
        [&](Self* instance) { return instance->initWithTexture(texture); });
}

Self* Self::createWithTexture(cocos2d::Texture2D* texture,
                              const cocos2d::Rect& rect, bool rotated) {
    return createInstance<Self>([&](Self* instance) {
        return instance->initWithTexture(texture, rect, rotated);
    });
}

Self* Self::createWithSpriteFrame(cocos2d::SpriteFrame* spriteFrame) {
    return createInstance<Self>(std::bind(&Self::initWithSpriteFrame,
                                          std::placeholders::_1, spriteFrame));
}

Self* Self::createWithSpriteFrameName(const std::string& spriteFrameName) {
    return createInstance<Self>(std::bind(&Self::initWithSpriteFrameName,
                                          std::placeholders::_1,
                                          spriteFrameName));
}

Self::SpriteWithHsv()
    : customState_(nullptr) {}

bool Self::initWithTexture(cocos2d::Texture2D* texture,
                           const cocos2d::Rect& rect, bool rotated) {
    if (not Super::initWithTexture(texture, rect, rotated)) {
        return false;
    }
    initShader();
    return true;
}

void Self::initShader() {
    customState_ = createHsvProgramState();
    setGLProgramState(customState_);
}
    
void Self::setGLProgramState(cocos2d::GLProgramState* glProgramState) {
#if COCOS2D_VERSION >= 0x00031400
    if (glProgramState != customState_) {
        return;
    }
#endif // COCOS2D_VERSION >= 0x00031400
    Super::setGLProgramState(glProgramState);
}


void Self::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform,
                std::uint32_t flags) {
    updateMatrix();
    Super::draw(renderer, transform, flags);
}

bool SpriteWithHsv::updateMatrix() {
    if (HsvProtocol::updateMatrix()) {
        getGLProgramState()->setUniformMat4("u_hsv", getHsvMatrix());
        return true;
    }
    return false;
}
} // namespace ee
