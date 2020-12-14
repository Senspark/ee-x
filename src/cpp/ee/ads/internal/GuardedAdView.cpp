#include "ee/ads/internal/GuardedAdView.hpp"

#include <ee/core/ObserverHandle.hpp>
#include <ee/core/Task.hpp>

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

Self::~GuardedAdView() = default;

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
    if (loading_) {
        // Waiting.
        co_return co_await ad_->load();
    }
    loading_ = true;
    loaded_ = co_await ad_->load();
    loading_ = false;
    co_return loaded_;
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