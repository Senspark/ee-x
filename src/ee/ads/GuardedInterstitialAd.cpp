#include "ee/ads/GuardedInterstitialAd.hpp"

#include <ee/ads/internal/AsyncHelper.hpp>
#include <ee/core/ObserverHandle.hpp>
#include <ee/coroutine/Task.hpp>

namespace ee {
namespace ads {
using Self = GuardedInterstitialAd;

Self::GuardedInterstitialAd(const std::shared_ptr<IInterstitialAd>& ad)
    : ad_(ad) {
    loading_ = false;
    loaded_ = false;
    displaying_ = false;

    handle_ = std::make_unique<ObserverHandle>();
    handle_->bind(*ad_).addObserver({
        .onLoaded =
            [this] {
                loaded_ = true;

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

Self::~GuardedInterstitialAd() {}

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
    auto result = co_await ad_->load();
    loading_ = false;
    co_return result;
}

Task<bool> Self::show() {
    if (not loaded_) {
        co_return false;
    }
    if (loading_) {
        co_return false;
    }
    if (displaying_) {
        // Waiting.
        co_return co_await ad_->show();
    }
    displaying_ = true;
    auto result = co_await ad_->show();
    displaying_ = false;
    if (not result) {
        // Failed to show, can use this ad again.
    } else {
        loaded_ = false;
    }
    co_return result;
}
} // namespace ads
} // namespace ee