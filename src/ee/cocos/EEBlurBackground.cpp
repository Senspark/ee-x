//
//  EEBlurBackground.cpp
//  ee-library
//
//  Created by Zinge on 6/24/16.
//
//

#include "EEBlurBackground.hpp"
#include "EEShaderUtils.hpp"

#include <2d/CCRenderTexture.h>
#include <base/CCDirector.h>
#include <renderer/CCRenderer.h>

namespace ee {
using Self = BlurBackground;

bool Self::init() {
    if (Super::init() == false) {
        return false;
    }

    renderScale_ = 1.0f;
    sigma_ = 2.0f;
    blurRadius_ = 4;
    useLinearSampling_ = false;
    rendererDirty_ = true;
    rendererInitialized_ = false;

    scheduleUpdate();
    return true;
}

void Self::updateRenderers() {
    if (rendererDirty_) {
        rendererDirty_ = false;
        resetRenderers();
        createRenderers();
    }
}

void Self::createRenderers() {
    CC_ASSERT(not rendererInitialized_);
    CC_ASSERT(horizontalRenderer_ == nullptr);
    CC_ASSERT(verticalRenderer_ == nullptr);

    auto&& winSize = _director->getWinSize();
    setContentSize(winSize);

    auto renderSize = winSize * renderScale_;

    // Make blur consistent between different content scale factors.
    renderSize = renderSize / CC_CONTENT_SCALE_FACTOR();

    int width = static_cast<int>(std::ceil(renderSize.width));
    int height = static_cast<int>(std::ceil(renderSize.height));
    renderSize_ = cocos2d::Size(width, height);

    auto createRenderer = [=] {
        auto renderer = cocos2d::RenderTexture::create(
            width, height, cocos2d::Texture2D::PixelFormat::RGBA8888,
            GL_DEPTH24_STENCIL8);
        renderer->setCascadeColorEnabled(true);
        renderer->setCascadeOpacityEnabled(true);
        return renderer;
    };

    horizontalRenderer_ = createRenderer();
    verticalRenderer_ = createRenderer();

    configHorizontalRenderer(horizontalRenderer_);
    configVerticalRenderer(verticalRenderer_);

    addChild(horizontalRenderer_, -1);
    addChild(verticalRenderer_, -1);

    rendererInitialized_ = true;
}

void Self::resetRenderers() {
    if (rendererInitialized_ == false) {
        return;
    }

    CC_ASSERT(horizontalRenderer_ != nullptr);
    CC_ASSERT(verticalRenderer_ != nullptr);

    horizontalRenderer_->removeFromParent();
    verticalRenderer_->removeFromParent();

    horizontalRenderer_ = nullptr;
    verticalRenderer_ = nullptr;

    rendererInitialized_ = false;
}

void Self::configRenderer(cocos2d::RenderTexture* renderer) {
    auto sprite = renderer->getSprite();
    auto texture = sprite->getTexture();

    if (renderScale_ < 1.0f) {
        texture->setAntiAliasTexParameters();
    } else {
        texture->setAliasTexParameters();
    }
}

void Self::configHorizontalRenderer(cocos2d::RenderTexture* renderer) {
    configRenderer(renderer);

    renderer->setKeepMatrix(true);
    renderer->setVisible(false);

    auto sprite = renderer->getSprite();
    auto texture = sprite->getTexture();
    auto textureWidth =
        static_cast<std::size_t>(texture->getContentSizeInPixels().width);

    sprite->setGLProgram(createHorizontalBlurProgram(
        textureWidth, blurRadius_, useLinearSampling_, sigma_));
    sprite->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
}

void Self::configVerticalRenderer(cocos2d::RenderTexture* renderer) {
    // Not precise.
    // renderer->setScale(CC_CONTENT_SCALE_FACTOR() / renderScale_);
    configRenderer(renderer);

    renderer->setKeepMatrix(false);

    auto&& winSize = _director->getWinSize();
    renderer->setScale(winSize.width / renderSize_.width,
                       winSize.height / renderSize_.height);

    auto sprite = renderer->getSprite();
    auto texture = sprite->getTexture();

    auto textureHeight =
        static_cast<std::size_t>(texture->getContentSizeInPixels().height);

    sprite->setGLProgram(createVerticalBlurProgram(textureHeight, blurRadius_,
                                                   useLinearSampling_, sigma_));
    sprite->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
}

Self* Self::setRenderScale(float scale) {
    if (renderScale_ == scale) {
        return this;
    }
    renderScale_ = scale;
    rendererDirty_ = true;
    return this;
}

Self* Self::setBlurRadius(std::size_t radius) {
    if (blurRadius_ == radius) {
        return this;
    }
    blurRadius_ = radius;
    rendererDirty_ = true;
    return this;
}

Self* Self::setSigma(float sigma) {
    if (sigma_ == sigma) {
        return this;
    }
    sigma_ = sigma;
    rendererDirty_ = true;
    return this;
}

Self* Self::setLinearSamplingEnabled(bool enabled) {
    if (useLinearSampling_ == enabled) {
        return this;
    }
    useLinearSampling_ = enabled;
    rendererDirty_ = true;
    return this;
}

Self* Self::setAutoRender(bool enabled) {
    if (enabled) {
        scheduleUpdate();
    } else {
        unscheduleUpdate();
    }
    return this;
}

Self* Self::render() {
    updateRenderers();

    setVisible(false);
    horizontalRenderer_->beginWithClear(0, 0, 0, 0);

    auto scene = _director->getRunningScene();
    scene->visit();

    horizontalRenderer_->end();
    setVisible(true);

    verticalRenderer_->beginWithClear(0, 0, 0, 0);
    horizontalRenderer_->getSprite()->visit();
    verticalRenderer_->end();

    // Force render immediately to prevent crashs when switching scenes.
    _director->getRenderer()->render();
    return this;
}

void Self::update(float delta) {
    if (isVisible()) {
        render();
    }
}

void Self::visit(cocos2d::Renderer* renderer,
                 const cocos2d::Mat4& parentTransforms,
                 std::uint32_t parentFlags) {
    Super::visit(renderer, parentTransforms, parentFlags);
}
} // namespace ee
