//
//  RewardedVideoInterface.cpp
//  ee_x
//
//  Created by Zinge on 10/5/17.
//
//

#include "RewardedVideoInterface.hpp"

namespace ee {
namespace ads {
using Self = RewardedVideoInterface;

Self::RewardedVideoInterface() {
    callback_ = nullptr;
}

Self::~RewardedVideoInterface() {}

void Self::setResultCallback(const RewardedVideoCallback& callback) {
    callback_ = callback;
}

void Self::setResult(bool result) {
    if (callback_) {
        callback_(result);
    }
}
} // namespace ads
} // namespace ee
