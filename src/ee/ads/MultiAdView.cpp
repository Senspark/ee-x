//
//  MultiBannerAd.cpp
//  ee_x
//
//  Created by Zinge on 10/18/17.
//
//

#include "ee/ads/MultiAdView.hpp"

#include <ee/core/NoAwait.hpp>
#include <ee/core/ObserverHandle.hpp>
#include <ee/core/SpinLock.hpp>
#include <ee/core/Task.hpp>

namespace ee {
namespace ads {
using Self = MultiAdView;

Self::MultiAdView() {
    anchor_ = std::make_pair(0, 0);
    position_ = std::make_pair(0, 0);
    visible_ = false;
    handle_ = std::make_unique<ObserverHandle>();
    locker_ = std::make_unique<SpinLock>();
}

Self::~MultiAdView() = default;

Self& Self::addItem(const std::shared_ptr<IAdView>& item) {
    items_.push_back(item);
    item->setVisible(visible_);
    (*handle_) //
        .bind(*item)
        .addObserver({
            .onLoaded =
                [this, item] {
                    std::unique_lock<SpinLock> lk(*locker_);
                    loadedItems_.insert(item);
                    lk.unlock();

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
    return *this;
}

void Self::destroy() {
    for (auto&& item : items_) {
        item->destroy();
    }
    items_.clear();
    handle_->clear();
}

bool Self::isLoaded() const {
    for (auto&& item : items_) {
        if (item->isLoaded()) {
            return true;
        }
    }
    return false;
}

Task<bool> Self::load() {
    bool result = false;
    for (auto&& item : items_) {
        if (item == activeItem_ && visible_) {
            // Ignore displaying item.
            continue;
        }
        std::unique_lock<SpinLock> lk(*locker_);
        if (loadedItems_.count(item) != 0) {
            // Already loaded and not displayed.
            continue;
        }
        lk.unlock();
        if (co_await item->load()) {
            result = true;
        }
    }
    co_return result;
}

std::pair<float, float> Self::getAnchor() const {
    return anchor_;
}

void Self::setAnchor(float x, float y) {
    anchor_ = std::make_pair(x, y);
    for (auto&& item : items_) {
        item->setAnchor(x, y);
    }
}

std::pair<float, float> Self::getPosition() const {
    return position_;
}

void Self::setPosition(float x, float y) {
    position_ = std::make_pair(x, y);
    for (auto&& item : items_) {
        item->setPosition(x, y);
    }
}

std::pair<float, float> Self::getSize() const {
    // Combined size of all ad views.
    int width = 0;
    int height = 0;
    for (auto&& item : items_) {
        int itemWidth, itemHeight;
        std::tie(itemWidth, itemHeight) = item->getSize();
        width = std::max(width, itemWidth);
        height = std::max(height, itemHeight);
    }
    return std::pair(width, height);
}

void Self::setSize(float width, float height) {
    for (auto&& item : items_) {
        item->setSize(width, height);
    }
}

bool Self::isVisible() const {
    return visible_;
}

void Self::setVisible(bool visible) {
    visible_ = visible;
    for (auto&& item : items_) {
        item->setVisible(false);
    }
    if (visible) {
        std::scoped_lock<SpinLock> lk(*locker_);
        if (loadedItems_.empty()) {
            for (auto&& item : items_) {
                if (item != activeItem_ && item->isLoaded()) {
                    activeItem_ = item;
                    break;
                }
            }
        } else {
            // Prefer to displaying loaded ad.
            for (auto&& item : items_) {
                if (loadedItems_.count(item) != 0) {
                    loadedItems_.erase(item);
                    activeItem_ = item;
                    break;
                }
            }
        }
        if (activeItem_) {
            activeItem_->setVisible(true);
        }
    } else {
        if (activeItem_) {
            // Reload the currently active ad.
            noAwait(activeItem_->load());
        }
    }
}
} // namespace ads
} // namespace ee
