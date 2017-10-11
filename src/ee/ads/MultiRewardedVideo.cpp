//
//  RewardedVideoList.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#include "ee/ads/MultiRewardedVideo.hpp"

namespace ee {
namespace ads {
using Self = MultiRewardedVideo;

Self::MultiRewardedVideo() {}

Self::~MultiRewardedVideo() {
    for (auto&& item : items_) {
        item->setResultCallback(nullptr);
    }
}

Self& Self::addItem(const std::shared_ptr<RewardedVideoInterface>& item) {
    items_.push_back(item);
    item->setResultCallback(
        std::bind(&Self::setResult, this, std::placeholders::_1));
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

bool Self::show() {
    for (auto&& item : items_) {
        if (item->show()) {
            return true;
        }
    }
    return false;
}
} // namespace ads
} // namespace ee
