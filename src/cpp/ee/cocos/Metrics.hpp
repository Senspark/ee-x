//
//  Metrics.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#ifndef EE_X_METRICS_HPP
#define EE_X_METRICS_HPP

#ifdef __cplusplus

#include <string>

#include "ee/cocos/CocosFwd.hpp"

namespace ee {
namespace cocos {
class Metrics final {
private:
    using Self = Metrics;

public:
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
} // namespace cocos
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_METRICS_HPP */
