//
//  IAdView.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#include "ee/ads/IAdView.hpp"
#include <ee/Core.hpp>
#include "cocos2d.h"

namespace ee {
namespace ads {
using Self = IAdView;

Self::IAdView() {}

Self::~IAdView() {}

void Self::setLoadCallback(const AdViewCallback& callback) {
    callback_ = callback;
}

void Self::setLoadResult(bool result) {
    if (callback_) {
        callback_(result);
    }
}

void Self::setPositionInPoints(float x, float y) {
    int&& xInPixels = (int)ee::Metrics::fromPoint(x).toPixel();
    int&& yInPixels = (int)ee::Metrics::fromPoint(y).toPixel();
    setPosition(xInPixels, yInPixels);
}

} // namespace ads
} // namespace ee
