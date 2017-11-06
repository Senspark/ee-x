//
//  MultiBannerAd.cpp
//  ee_x
//
//  Created by Zinge on 10/18/17.
//
//

#include "ee/ads/MultiAdView.hpp"

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
}

Self::~MultiAdView() {}

Self& Self::addItem(const std::shared_ptr<AdViewInterface>& item) {
    items_.push_back(item);
    item->setLoadCallback([this, item](bool result) {
        if (result) {
            bool displayed = false;
            if (visible_) {
                if (not new_) {
                    // Display new item.
                    activeItem_ = item;
                    activeItem_->setVisible(true);
                    displayed = true;
                    new_ = true;
                }
            }
            if (not displayed) {
                loadedItems_.insert(item);
            }
        }
    });
    return *this;
}

bool Self::isLoaded() const {
    return not loadedItems_.empty();
}

void Self::load() {
    for (auto&& item : items_) {
        if (item == activeItem_ && visible_) {
            // Ignore displaying item.
            continue;
        }
        if (loadedItems_.count(item) == 0) {
            // Force old views to load.
            item->load();
        }
    }
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

void Self::setVisible(bool visible) {
    if (visible == visible_) {
        return;
    }
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
