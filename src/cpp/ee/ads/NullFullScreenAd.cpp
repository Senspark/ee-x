#include "ee/ads/NullFullScreenAd.hpp"

#include <ee/core/Task.hpp>

namespace ee {
namespace ads {
using Self = NullFullScreenAd;

void Self::destroy() {}

bool Self::isLoaded() const {
    return false;
}

Task<bool> Self::load() {
    co_return false;
}

Task<FullScreenAdResult> Self::show() {
    co_return FullScreenAdResult::Failed;
}
} // namespace ads
} // namespace ee
