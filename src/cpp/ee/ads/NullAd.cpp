//
//  NullAd.cpp
//  Pods
//
//  Created by eps on 2/15/21.
//

#include "ee/ads/NullAd.hpp"

#include <ee/core/Task.hpp>

namespace ee {
namespace ads {
using Self = NullAd;

void Self::destroy() {}

bool Self::isLoaded() const {
    return false;
}

Task<bool> Self::load() {
    co_return false;
}
} // namespace ads
} // namespace ee
