//
//  MultiBannerAd.cpp
//  ee_x
//
//  Created by Zinge on 10/18/17.
//
//

#include "ee/ads/MultiBannerAd.hpp"

#include <ee/core/NoAwait.hpp>
#include <ee/core/ObserverHandle.hpp>
#include <ee/core/Task.hpp>

namespace ee {
namespace ads {
using Self = MultiBannerAd;

Self::MultiBannerAd() {
    visible_ = false;
    anchor_ = std::pair(0, 0);
    position_ = std::pair(0, 0);
    handle_ = std::make_unique<ObserverHandle>();
}

Self::~MultiBannerAd() = default;

Self& Self::addItem(const std::shared_ptr<IBannerAd>& item) {
    items_.push_back(item);
    item->setVisible(visible_);
    (*handle_) //
        .bind(*item)
        .addObserver({
            .onLoaded =
                [this, item] {
                    loadedItems_.insert(item);
                    invalidate();

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
    activeItem_.reset();
    loadedItems_.clear();
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
        if (loadedItems_.count(item) != 0) {
            // Already loaded.
            continue;
        }
        if (co_await item->load()) {
            result = true;
        }
    }
    co_return result;
}

bool Self::isVisible() const {
    return visible_;
}

void Self::setVisible(bool visible) {
    visible_ = visible;
    invalidate();
    for (auto&& item : items_) {
        if (loadedItems_.count(item) == 0) {
            // Load in background.
            noAwait(item->load());
        }
    }
}

std::pair<float, float> Self::getAnchor() const {
    return anchor_;
}

void Self::setAnchor(float x, float y) {
    anchor_ = std::pair(x, y);
    for (auto&& item : items_) {
        item->setAnchor(x, y);
    }
}

std::pair<float, float> Self::getPosition() const {
    return position_;
}

void Self::setPosition(float x, float y) {
    position_ = std::pair(x, y);
    for (auto&& item : items_) {
        item->setPosition(x, y);
    }
}

std::pair<float, float> Self::getSize() const {
    // Combined size of all ad views.
    float width = 0;
    float height = 0;
    for (auto&& item : items_) {
        float itemWidth, itemHeight;
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

void Self::invalidate() {
    auto lastActiveItem = activeItem_;
    for (auto&& item : items_) {
        if (loadedItems_.count(item) != 0) {
            activeItem_ = item;
            break;
        }
    }
    if (activeItem_) {
        activeItem_->setVisible(visible_);
    }
    if (lastActiveItem != nullptr && lastActiveItem != activeItem_) {
        lastActiveItem->setVisible(false);
    }
}
} // namespace ads
} // namespace ee
