//
//  InterstitialAdInterface.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#include "InterstitialAdInterface.hpp"

namespace ee {
namespace ads {
using Self = InterstitialAdInterface;

Self::InterstitialAdInterface() {
    callback_ = nullptr;
}

Self::~InterstitialAdInterface() {}

void Self::setResultCallback(const InterstitialAdCallback& callback) {
    callback_ = callback;
}

void Self::setDone() {
    if (callback_) {
        callback_();
    }
}
} // namespace ads
} // namespace ee
