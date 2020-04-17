//
//  NullInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/27/17.
//
//

#include "ee/ads/NullInterstitialAd.hpp"

#include <ee/coroutine/Task.hpp>

namespace ee {
namespace ads {
using Self = NullInterstitialAd;

bool Self::isLoaded() const {
    return false;
}

Task<bool> Self::load() {
    co_return false;
}

Task<bool> Self::show() {
    co_return false;
}
} // namespace ads
} // namespace ee
