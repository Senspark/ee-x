//
//  LazyAdView.cpp
//  ee-x-d542b565
//
//  Created by eps on 1/7/21.
//

#include "ee/services/internal/LazyBannerAd.hpp"

#include <ee/core/NoAwait.hpp>
#include <ee/core/ObserverHandle.hpp>
#include <ee/core/Task.hpp>

namespace ee {
namespace services {
using Self = LazyBannerAd;

Self::LazyBannerAd() {
    visible_ = false;
    anchor_ = std::pair(0, 0);
    position_ = std::pair(0, 0);
    size_ = std::nullopt;
    handle_ = std::make_unique<ObserverHandle>();
}

Self::~LazyBannerAd() = default;

void Self::setAd(const std::shared_ptr<IBannerAd>& ad) {
    handle_->clear();
    if (ad == nullptr) {
        return;
    }
    (*handle_) //
        .bind(*ad)
        .addObserver({
            .onLoaded =
                [this] {
                    if (visible_) {
                        ad_->setVisible(false);
                        ad_->setVisible(true);
                    }

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
    ad_ = ad;
    ad->setVisible(visible_);
    ad->setAnchor(anchor_.first, anchor_.second);
    ad->setPosition(position_.first, position_.second);
    if (size_.has_value()) {
        ad->setSize(size_->first, size_->second);
    }
    noAwait(ad_->load());
}

void Self::destroy() {
    if (ad_ != nullptr) {
        ad_->destroy();
    }
    handle_->clear();
}

bool Self::isLoaded() const {
    if (ad_ == nullptr) {
        return false;
    }
    return ad_->isLoaded();
}

Task<bool> Self::load() {
    if (ad_ == nullptr) {
        co_return false;
    }
    co_return co_await ad_->load();
}

std::pair<float, float> Self::getAnchor() const {
    return anchor_;
}

void Self::setAnchor(float x, float y) {
    anchor_ = std::pair(x, y);
    if (ad_ != nullptr) {
        ad_->setAnchor(x, y);
    }
}

std::pair<float, float> Self::getPosition() const {
    return position_;
}

void Self::setPosition(float x, float y) {
    position_ = std::pair(x, y);
    if (ad_ != nullptr) {
        ad_->setPosition(x, y);
    }
}

std::pair<float, float> Self::getSize() const {
    if (size_.has_value()) {
        return size_.value();
    }
    if (ad_ != nullptr) {
        return ad_->getSize();
    }
    return std::pair(0, 0);
}

void Self::setSize(float width, float height) {
    size_ = std::pair(width, height);
    if (ad_ != nullptr) {
        ad_->setSize(width, height);
    }
}

bool Self::isVisible() const {
    return visible_;
}

void Self::setVisible(bool visible) {
    visible_ = visible;
    if (ad_ != nullptr) {
        ad_->setVisible(visible);
    }
}
} // namespace services
} // namespace ee
