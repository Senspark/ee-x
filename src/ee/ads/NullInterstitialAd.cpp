//
//  NullInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/27/17.
//
//

#include "ee/ads/NullInterstitialAd.hpp"

namespace ee {
namespace ads {
using Self = NullInterstitialAd;

bool Self::isLoaded() const {
    return false;
}

void Self::load() {
    // No op.
}

bool Self::show() {
    return false;
}
} // namespace ads
} // namespace ee
