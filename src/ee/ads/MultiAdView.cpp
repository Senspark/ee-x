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
    customSize_ = false;
}

Self::~MultiAdView() {}

Self& Self::addItem(const std::shared_ptr<AdViewInterface>& item) {
    items_.push_back(item);
    item->setLoadCallback([this, item](bool result) {
        if (result) {
            if (activeItem_ == nullptr) {
                activeItem_ = item;
                if (visible_) {
                    activeItem_->setVisible(visible_);
                }
            }
        }
    });
    return *this;
}

bool Self::isLoaded() const {
    for (auto&& item : items_) {
        if (item->isLoaded()) {
            return true;
        }
    }
    return false;
}

void Self::load() {
    for (auto&& item : items_) {
        if (not item->isLoaded()) {
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
    findActiveItem();
    if (activeItem_) {
        return activeItem_->getSize();
    }
    if (customSize_) {
        return size_;
    }
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
    customSize_ = true;
    for (auto&& item : items_) {
        item->setSize(width, height);
    }
}

void Self::setVisible(bool visible) {
    visible_ = visible;
    for (auto&& item : items_) {
        item->setVisible(false);
    }
    if (visible) {
        findActiveItem();
        if (activeItem_) {
            activeItem_->setVisible(true);
        }
    }
}

void Self::findActiveItem() const {
    if (activeItem_) {
        return;
    }
    for (auto&& item : items_) {
        if (item->isLoaded()) {
            activeItem_ = item;
            break;
        }
    }
}
} // namespace ads
} // namespace ee
