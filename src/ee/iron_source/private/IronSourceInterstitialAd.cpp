//
//  IronSourceInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include "ee/iron_source/private/IronSourceInterstitialAd.hpp"

#include <cassert>

#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/Logger.hpp>

#include "ee/iron_source/IronSourceBridge.hpp"

namespace ee {
namespace iron_source {
using Self = InterstitialAd;

Self::InterstitialAd(const Logger& logger, Bridge* plugin,
                     const std::string& placementId)
    : logger_(logger) {
    logger_.debug(__PRETTY_FUNCTION__);
    plugin_ = plugin;
    placementId_ = placementId;
}

Self::~InterstitialAd() {
    logger_.debug("%s: begin", __PRETTY_FUNCTION__);
    plugin_->destroyInterstitialAd(placementId_);
    logger_.debug("%s: end", __PRETTY_FUNCTION__);
}

bool Self::isLoaded() const {
    return plugin_->hasInterstitial();
}

void Self::load() {
    // No op.
    plugin_->loadInterstitial();
}

bool Self::show() {
    if (not plugin_->showInterstitial(placementId_)) {
        return false;
    }
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.startInterstitialAd([this]() {
        this->load();
        this->setDone();
    });
    assert(successful);
    return true;
}
} // namespace iron)source
} // namespace ee
