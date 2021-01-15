//
//  Metrics.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#include "ee/cocos/Metrics.hpp"

#ifdef EE_X_COCOS_CPP
#include <base/CCDirector.h>
#include <platform/CCGLView.h>

#include <ee/core/Platform.hpp>

namespace ee {
namespace cocos {
using Self = Metrics;

namespace {
/// Pixel / DP = ratio.
float getPixelToDpRatio() {
    return getDensity();
}

/// Pixel / Point = ratio.
float getPixelToPointRatio() {
    auto&& director = cocos2d::Director::getInstance();
    auto&& glView = director->getOpenGLView();
    auto&& frameSize = glView->getFrameSize();
    auto&& winSize = director->getWinSize();
    auto&& policy = glView->getResolutionPolicy();
    switch (policy) {
    case ResolutionPolicy::FIXED_WIDTH:
        return frameSize.width / winSize.width;
    case ResolutionPolicy::FIXED_HEIGHT:
        return frameSize.height / winSize.height;
    }
    // Unsupported resolution policy.
    CC_ASSERT(false);
    return 1.0f;
}
} // namespace

Self Self::fromPoint(float value) {
    auto pixelToDp = getPixelToDpRatio();
    auto pixelToPoint = getPixelToPointRatio();
    return Self(value * pixelToPoint, pixelToDp, pixelToPoint);
}

Self Self::fromPixel(float value) {
    auto pixelToDp = getPixelToDpRatio();
    auto pixelToPoint = getPixelToPointRatio();
    return Self(value, pixelToDp, pixelToPoint);
}

Self Self::fromDip(float value) {
    auto pixelToDp = getPixelToDpRatio();
    auto pixelToPoint = getPixelToPointRatio();
    return Self(value * pixelToDp, pixelToDp, pixelToPoint);
}

Self::Metrics(float pixels, float pixelToDp, float pixelToPoint)
    : pixels_(pixels)
    , pixelToDp_(pixelToDp)
    , pixelToPoint_(pixelToPoint) {}

float Self::toPoint() const {
    return pixels_ / pixelToPoint_;
}

float Self::toPixel() const {
    return pixels_;
}

float Self::toDip() const {
    return pixels_ / pixelToDp_;
}
} // namespace cocos
} // namespace ee

#endif // EE_X_COCOS_CPP