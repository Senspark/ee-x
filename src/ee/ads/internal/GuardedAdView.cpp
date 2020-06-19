#include "ee/ads/internal/GuardedAdView.hpp"

#include <mutex>

#include <ee/core/ObserverHandle.hpp>
#include <ee/core/SpinLock.hpp>
#include <ee/core/SwitchToUiThread.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>

namespace ee {
namespace ads {
using Self = GuardedAdView;

Self::GuardedAdView(const std::shared_ptr<IAdView>& ad)
    : ad_(ad) {
    loading_ = false;
    loaded_ = false;

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

Self::~GuardedAdView() = default;

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
    lock.unlock();
    co_await SwitchToUiThread();
    lock.lock();
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

std::pair<float, float> Self::getAnchor() const {
    return ad_->getAnchor();
}

void Self::setAnchor(float x, float y) {
    ad_->setAnchor(x, y);
}

std::pair<float, float> Self::getPosition() const {
    return ad_->getPosition();
}

void Self::setPosition(float x, float y) {
    ad_->setPosition(x, y);
}

std::pair<float, float> Self::getSize() const {
    return ad_->getSize();
}

void Self::setSize(float width, float height) {
    ad_->setSize(width, height);
}

bool Self::isVisible() const {
    return ad_->isVisible();
}

void Self::setVisible(bool visible) {
    ad_->setVisible(visible);
}
} // namespace ads
} // namespace ee
