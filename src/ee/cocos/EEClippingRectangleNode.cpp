//
//  EEClippingRectangleNode.cpp
//  ee-library
//
//  Created by Zinge on 9/1/17.
//
//

#include "EEClippingRectangleNode.hpp"
#include "EEUtils.hpp"

#include <base/CCDirector.h>
#include <renderer/CCRenderer.h>

namespace ee {
using Self = ClippingRectangleNode;

Self* Self::create() {
    return createInstance<Self>(
        [](Self* instance) { return instance->init(); });
}

Self* Self::create(const cocos2d::Rect& region) {
    auto result = create();
    result->setClippingRegion(region);
    return result;
}

Self::ClippingRectangleNode() {
    hasPreviousScissorRect_ = false;
    previousScissorRect_ = cocos2d::Rect::ZERO;
}

Self::~ClippingRectangleNode() {}

void Self::visit(cocos2d::Renderer* renderer,
                 const cocos2d::Mat4& parentTransform,
                 std::uint32_t parentFlags) {
    _beforeVisitCmdScissor.init(getGlobalZOrder());
    _beforeVisitCmdScissor.func = std::bind(&Self::onBeforeVisitScissor, this);
    renderer->addCommand(std::addressof(_beforeVisitCmdScissor));

    // Disable Super::visit.
    Node::visit(renderer, parentTransform, parentFlags);

    _afterVisitCmdScissor.init(getGlobalZOrder());
    _afterVisitCmdScissor.func = std::bind(&Self::onAfterVisitScissor, this);
    renderer->addCommand(std::addressof(_afterVisitCmdScissor));
}

void Self::onBeforeVisitScissor() {
    if (not isClippingEnabled()) {
        return;
    }
    auto glView = _director->getOpenGLView();

    // http://stackoverflow.com/questions/14645266/glscissor-call-inside-another-glscissor
    hasPreviousScissorRect_ = glIsEnabled(GL_SCISSOR_TEST);
    if (hasPreviousScissorRect_) {
        previousScissorRect_ = glView->getScissorRect();
    }
    if (not hasPreviousScissorRect_) {
        glEnable(GL_SCISSOR_TEST);
    }
    auto scaleX = getScaleX();
    auto scaleY = getScaleY();
    auto parent = getParent();
    while (parent != nullptr) {
        scaleX *= parent->getScaleX();
        scaleY *= parent->getScaleY();
        parent = parent->getParent();
    }

    auto position = convertToWorldSpace(getClippingRegion().origin);
    auto x = position.x;
    auto y = position.y;
    auto w = getClippingRegion().size.width * scaleX;
    auto h = getClippingRegion().size.height * scaleY;
    if (hasPreviousScissorRect_) {
        // Recursive clipping.
        if (x < previousScissorRect_.getMinX()) {
            x = previousScissorRect_.getMinX();
        }
        if (y < previousScissorRect_.getMinY()) {
            y = previousScissorRect_.getMinY();
        }
        if (x + w > previousScissorRect_.getMaxX()) {
            w = std::max(0.0f, previousScissorRect_.getMaxX() - x);
        }
        if (y + h > previousScissorRect_.getMaxY()) {
            h = std::max(0.0f, previousScissorRect_.getMaxY() - y);
        }
    }
    glView->setScissorInPoints(x, y, w, h);
}

void Self::onAfterVisitScissor() {
    if (not isClippingEnabled()) {
        return;
    }
    if (hasPreviousScissorRect_) {
        auto glView = _director->getOpenGLView();
        glView->setScissorInPoints(
            previousScissorRect_.origin.x, previousScissorRect_.origin.y,
            previousScissorRect_.size.width, previousScissorRect_.size.height);
    } else {
        glDisable(GL_SCISSOR_TEST);
    }
}
} // namespace ee
