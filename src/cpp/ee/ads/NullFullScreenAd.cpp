#include "ee/ads/NullFullScreenAd.hpp"

#include <ee/core/Task.hpp>

namespace ee {
namespace ads {
using Self = NullFullScreenAd;

Task<AdResult> Self::show() {
    co_return AdResult::Failed;
}
} // namespace ads
} // namespace ee
