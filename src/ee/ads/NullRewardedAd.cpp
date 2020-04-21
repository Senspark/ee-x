//
//  NullRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/27/17.
//
//

#include "ee/ads/NullRewardedAd.hpp"

#include <ee/coroutine/Task.hpp>

namespace ee {
namespace ads {
using Self = NullRewardedAd;

void Self::destroy() {}

bool Self::isLoaded() const {
    return false;
}

Task<bool> Self::load() {
    co_return false;
}

Task<IRewardedAdResult> Self::show() {
    co_return IRewardedAdResult::Failed;
}
} // namespace ads
} // namespace ee
