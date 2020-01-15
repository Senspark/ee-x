//
//  RewardedVideoList.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#include "ee/ads/MultiRewardedVideo.hpp"

#include <ee/core/Logger.hpp>

namespace ee {
namespace ads {
using Self = MultiRewardedVideo;

Self::MultiRewardedVideo()
    : Self(Logger::getSystemLogger()) {}

Self::MultiRewardedVideo(const Logger& logger)
    : Super(logger) {}

Self::~MultiRewardedVideo() {}

Self& Self::addItem(const std::shared_ptr<IRewardedVideo>& item) {
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

    // Fix AdMob consumes other ads' callbacks.
    assignCallbacks();

    for (auto&& item : items_) {
        if (not displayed) {
            if (item->show()) {
                displayed = true;
                continue;
            }
        }
        if (not item->isLoaded()) {
            item->load();
        }
    }

    if (not displayed) {
        clearCallbacks();
    }

    return displayed;
}

void Self::assignCallbacks() {
    for (auto&& item : items_) {
        item->setResultCallback([this](bool result) {
            setResult(result);
            clearCallbacks();
        });
    }
}

void Self::clearCallbacks() {
    for (auto&& item : items_) {
        item->setResultCallback(nullptr);
    }
}

void Self::setOnClickedCallback(const OnRewardClickedCallback& callback) {
    for (auto&& item : items_) {
        item->setOnClickedCallback(callback);
    }
}
} // namespace ads
} // namespace ee
