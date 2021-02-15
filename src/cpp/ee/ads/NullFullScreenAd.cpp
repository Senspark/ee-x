#include "ee/ads/NullFullScreenAd.hpp"

#include <ee/core/Task.hpp>

namespace ee {
namespace ads {
using Self = NullFullScreenAd;

Task<FullScreenAdResult> Self::show() {
    co_return FullScreenAdResult::Failed;
}
} // namespace ads
} // namespace ee
