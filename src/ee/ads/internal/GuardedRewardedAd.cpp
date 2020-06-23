#include "ee/ads/internal/GuardedRewardedAd.hpp"

#include <mutex>

#include <ee/ads/internal/AsyncHelper.hpp>
#include <ee/core/ObserverHandle.hpp>
#include <ee/core/SpinLock.hpp>
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
                std::unique_lock<SpinLock> lock(*lock_);
                loaded_ = true;
                lock.unlock();

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

    lock_ = std::make_unique<SpinLock>();
}

Self::~GuardedRewardedAd() = default;

void Self::destroy() {
    ad_->destroy();
    handle_->clear();
}

bool Self::isLoaded() const {
    std::scoped_lock<SpinLock> lock(*lock_);
    return loaded_;
}

Task<bool> Self::load() {
    std::unique_lock<SpinLock> lock(*lock_);
    if (loaded_) {
        lock.unlock();
        co_return true;
    }
    if (displaying_) {
        lock.unlock();
        co_return false;
    }
    if (loading_) {
        // Waiting.
        lock.unlock();
        co_return co_await ad_->load();
    }
    loading_ = true;
    lock.unlock();
    auto result = co_await ad_->load();
    lock.lock();
    loading_ = false;
    lock.unlock();
    co_return result;
}

Task<IRewardedAdResult> Self::show() {
    std::unique_lock<SpinLock> lock(*lock_);
    if (not loaded_) {
        lock.unlock();
        co_return IRewardedAdResult::Failed;
    }
    if (loading_) {
        lock.unlock();
        co_return IRewardedAdResult::Failed;
    }
    if (displaying_) {
        // Waiting.
        lock.unlock();
        co_return co_await ad_->show();
    }
    displaying_ = true;
    lock.unlock();
    auto result = co_await ad_->show();
    lock.lock();
    displaying_ = false;
    if (result == IRewardedAdResult::Failed) {
        // Failed to show, can use this ad again.
    } else {
        loaded_ = false;
    }
    lock.unlock();
    co_return result;
}
} // namespace ads
} // namespace ee
