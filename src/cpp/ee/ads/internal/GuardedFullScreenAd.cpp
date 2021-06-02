#include "ee/ads/internal/GuardedFullScreenAd.hpp"

#include <ee/core/Task.hpp>

namespace ee {
namespace ads {
using Self = GuardedFullScreenAd;

Self::GuardedFullScreenAd(const std::shared_ptr<IFullScreenAd>& ad,
                          const std::shared_ptr<ICapper>& capper,
                          const std::shared_ptr<IRetrier>& retrier)
    : GuardedAd(ad, capper, retrier)
    , ad_(ad) {
    displaying_ = false;
}

Self::~GuardedFullScreenAd() = default;

bool Self::isDisplaying() const {
    return displaying_;
}

Task<AdResult> Self::show() {
    if (not isLoaded()) {
        co_return AdResult::Failed;
    }
    if (isLoading()) {
        co_return AdResult::Failed;
    }
    if (isDisplaying()) {
        co_return AdResult::Failed;
    }
    displaying_ = true;
    loaded_ = false;
    auto result = co_await ad_->show();
    displaying_ = false;
    co_return result;
}
} // namespace ads
} // namespace ee
