//
//  Metrics.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#ifndef EE_X_CORE_METRICS_HPP
#define EE_X_CORE_METRICS_HPP

namespace ee {
namespace core {
class Metrics final {
private:
    using Self = Metrics;

public:
    /// Initializes the metrics.
    /// @param[in] ratio Pixel to (cocos2d-x) point ratio, should pass
    /// cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize().width
    /// / cocos2d::Director::getInstance()->getWinSize().width (FIXED_WIDTH
    /// policy).
    static void initialize(float ratio);

    /// Gets the screen density, i.e. pixel to dp ratio.
    static float getDensity();

    static Self fromPoint(float value);
    static Self fromPixel(float value);

    /// Density independent pixels.
    static Self fromDip(float value);

    float toPoint() const;
    float toPixel() const;
    float toDip() const;

private:
    explicit Metrics(float pixels, float pixelToDp, float pixelToPoint);

    float pixels_;
    float pixelToDp_;
    float pixelToPoint_;
};
} // namespace core

using core::Metrics;
} // namespace ee

#endif /* EE_X_CORE_METRICS_HPP */
