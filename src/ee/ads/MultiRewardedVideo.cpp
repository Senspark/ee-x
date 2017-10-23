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

Self::~MultiRewardedVideo() {}

Self& Self::addItem(const std::shared_ptr<RewardedVideoInterface>& item) {
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

bool Self::show() {
    bool displayed = false;
    for (auto&& item : items_) {
        if (not displayed) {
            item->setResultCallback([this, item](bool result) {
                setResult(result);
                item->setResultCallback(nullptr);
            });
            if (item->show()) {
                displayed = true;
                continue;
            }
        }
        if (not item->isLoaded()) {
            item->load();
        }
    }
    return displayed;
}
} // namespace ads
} // namespace ee
