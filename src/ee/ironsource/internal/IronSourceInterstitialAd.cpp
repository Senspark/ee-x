//
//  IronSourceInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include <cassert>

#include "ee/ads/internal/MediationManager.hpp"
#include "ee/core/Logger.hpp"
#include "ee/ironsource/IronSourceBridge.hpp"
#include "ee/ironsource/internal/IronSourceInterstitialAd.hpp"

namespace ee {
namespace ironsource {
using Self = InterstitialAd;

Self::InterstitialAd(IronSource* plugin, const std::string& placementId) {
    Logger::getSystemLogger().debug(__PRETTY_FUNCTION__);
    plugin_ = plugin;
    placementId_ = placementId;
}

Self::~InterstitialAd() {
    Logger::getSystemLogger().debug("%s: begin", __PRETTY_FUNCTION__);
    plugin_->destroyInterstitialAd(placementId_);
    Logger::getSystemLogger().debug("%s: end", __PRETTY_FUNCTION__);
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
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.startInterstitialAd([this]()
                                                    {
                                                        this->load();
                                                        this->setDone();
                                                    });
    assert(successful);
    return true;
}
} // namespace ironsource
} // namespace ee
