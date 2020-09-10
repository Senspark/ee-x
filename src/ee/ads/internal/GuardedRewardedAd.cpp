#include "ee/ads/internal/GuardedRewardedAd.hpp"

#include <ee/ads/internal/AsyncHelper.hpp>
#include <ee/core/ObserverHandle.hpp>
#include <ee/core/Task.hpp>

namespace ee {
namespace ads {
using Self = GuardedRewardedAd;

Self::GuardedRewardedAd(const std::shared_ptr<IRewardedAd>& ad)
    : ad_(ad) {
    loading_ = false;
    loaded_ = false;
    displaying_ = false;

    handle_ = std::make_unique<ObserverHandle>();
    handle_->bind(*ad_).addObserver({
        .onLoaded =
            [this] {
                // Propagation.
                dispatchEvent([](auto&& observer) {
                    if (observer.onLoaded) {
                        observer.onLoaded();
                    }
                });
            },
        .onClicked =
            [this] {
                // Propagation.
                dispatchEvent([](auto&& observer) {
                    if (observer.onClicked) {
                        observer.onClicked();
                    }
                });
            },
    });
}

Self::~GuardedRewardedAd() = default;

void Self::destroy() {
    ad_->destroy();
    handle_->clear();
}

bool Self::isLoaded() const {
    return loaded_;
}

Task<bool> Self::load() {
    if (loaded_) {
        co_return true;
    }
    if (displaying_) {
        co_return false;
    }
    if (loading_) {
        // Waiting.
        co_return co_await ad_->load();
    }
    loading_ = true;
    loaded_ = co_await ad_->load();
    loading_ = false;
    co_return loaded_;
}

Task<IRewardedAdResult> Self::show() {
    if (not loaded_) {
        co_return IRewardedAdResult::Failed;
    }
    if (loading_) {
        co_return IRewardedAdResult::Failed;
    }
    if (displaying_) {
        // Waiting.
        co_return co_await ad_->show();
    }
    displaying_ = true;
    auto result = co_await ad_->show();
    displaying_ = false;
    if (result == IRewardedAdResult::Failed) {
        // Failed to show, can use this ad again.
    } else {
        loaded_ = false;
    }
    co_return result;
}
} // namespace ads
} // namespace ee
