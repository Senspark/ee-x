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

Self::MultiAdView() {}

Self::~MultiAdView() {}

Self& Self::addItem(const std::shared_ptr<AdViewInterface>& item) {
    items_.push_back(item);
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

std::pair<int, int> Self::getPosition() const {
    findActiveItem();
    if (activeItem_) {
        return activeItem_->getPosition();
    }
    return std::make_pair(0, 0);
}

void Self::setPosition(int x, int y) {
    for (auto&& item : items_) {
        item->setPosition(x, y);
    }
}

std::pair<int, int> Self::getSize() const {
    findActiveItem();
    if (activeItem_) {
        return activeItem_->getSize();
    }
    return std::make_pair(0, 0);
}

void Self::setSize(int width, int height) {
    for (auto&& item : items_) {
        item->setSize(width, height);
    }
}

void Self::setVisible(bool visible) {
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
            item->load();
        }
    }
}
} // namespace ads
} // namespace ee
