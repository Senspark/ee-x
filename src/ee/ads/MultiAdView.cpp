//
//  MultiBannerAd.cpp
//  ee_x
//
//  Created by Zinge on 10/18/17.
//
//

#include "ee/ads/MultiAdView.hpp"

#include <ee/core/ObserverHandle.hpp>
#include <ee/coroutine/Task.hpp>

namespace ee {
namespace ads {
using Self = MultiAdView;

Self::MultiAdView() {
    anchor_ = std::make_pair(0, 0);
    position_ = std::make_pair(0, 0);
    size_ = std::make_pair(0, 0);
    visible_ = false;
    useCustomSize_ = false;
    new_ = false;
    handle_ = std::make_unique<ObserverHandle>();
}

Self::~MultiAdView() {}

Self& Self::addItem(const std::shared_ptr<IAdView>& item) {
    items_.push_back(item);
    item->setVisible(visible_);
    (*handle_) //
        .bind(*item)
        .addObserver({
            .onLoaded =
                [this, item] {
                    bool displayed = false;
                    if (visible_) {
                        /*
                         Uncomment to display loaded item immediately.
                        if (not new_) {
                            // Hide old item.
                            if (activeItem_) {
                                activeItem_->setVisible(false);
                            }
                            // Display new item.
                            activeItem_ = item;
                            activeItem_->setVisible(true);
                            displayed = true;
                            new_ = true;
                        }
                         */
                    }
                    if (not displayed) {
                        loadedItems_.insert(item);
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
    return not loadedItems_.empty();
}

Task<bool> Self::load() {
    bool result = false;
    for (auto&& item : items_) {
        if (item == activeItem_ && visible_) {
            // Ignore displaying item.
            continue;
        }
        if (loadedItems_.count(item) == 0) {
            // Force old views to load.
            if (co_await item->load()) {
                result = true;
            }
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

std::pair<int, int> Self::getPosition() const {
    return position_;
}

void Self::setPosition(int x, int y) {
    position_ = std::make_pair(x, y);
    for (auto&& item : items_) {
        item->setPosition(x, y);
    }
}

std::pair<int, int> Self::getSize() const {
    if (useCustomSize_) {
        return size_;
    }
    // Combined size of all ad views.
    int width = 0;
    int height = 0;
    for (auto&& item : items_) {
        int itemWidth, itemHeight;
        std::tie(itemWidth, itemHeight) = item->getSize();
        width = std::max(width, itemWidth);
        height = std::max(height, itemHeight);
    }
    return std::tie(width, height);
}

void Self::setSize(int width, int height) {
    size_ = std::make_pair(width, height);
    useCustomSize_ = true;
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
        new_ = false;
        if (not loadedItems_.empty()) {
            // Swap new item.
            for (auto&& item : items_) {
                if (loadedItems_.count(item) != 0) {
                    activeItem_ = item;
                    break;
                }
            }
            loadedItems_.erase(activeItem_);
            new_ = true;
        }
        if (activeItem_) {
            activeItem_->setVisible(true);
        }
    }
}
} // namespace ads
} // namespace ee
