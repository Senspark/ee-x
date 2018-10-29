//
//  IAdView.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#include "ee/ads/IAdView.hpp"

namespace ee {
namespace ads {
using Self = IAdView;

void Self::setLoadCallback(const AdViewCallback& callback) {
    callback_ = callback;
}

void Self::setLoadResult(bool result) {
    if (callback_) {
        callback_(result);
    }
}
} // namespace ads
} // namespace ee
