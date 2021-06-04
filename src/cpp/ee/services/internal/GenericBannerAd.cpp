//
//  CocosAdView.cpp
//  Pods
//
//  Created by eps on 6/18/20.
//

#include "ee/services/internal/GenericBannerAd.hpp"

#ifdef EE_X_COCOS_CPP
#include <base/CCDirector.h>

#include <ee/cocos/Metrics.hpp>
#include <ee/core/Task.hpp>

namespace ee {
namespace services {
using Self = GenericBannerAd;

Self::GenericBannerAd(const std::shared_ptr<IBannerAd>& ad,
                      const std::shared_ptr<ads::ICapper>& loadCapper,
                      const std::shared_ptr<ads::IRetrier>& loadRetrier)
    : GenericAd(ad, loadCapper, loadRetrier)
    , ad_(ad) {
    ad_->setAnchor(0.0f, 1.0f); // Reserved anchor.
    metrics_ = std::make_unique<Metrics>(Metrics::fromPoint(1));
    sceneHeight_ = cocos2d::Director::getInstance()->getWinSize().height;
}

Self::~GenericBannerAd() = default;

std::pair<float, float> Self::getAnchor() const {
    auto&& [x, y] = ad_->getAnchor();
    return std::pair(x, 1 - y);
}

void Self::setAnchor(float x, float y) {
    ad_->setAnchor(x, 1.0f - y);
}

std::pair<float, float> Self::getPosition() const {
    auto&& [x, y] = ad_->getPosition();
    return std::pair(x / metrics_->toPixel(),
                     sceneHeight_ - y / metrics_->toPixel());
}

void Self::setPosition(float x, float y) {
    ad_->setPosition(metrics_->toPixel() * x,
                     metrics_->toPixel() * (sceneHeight_ - y));
}

std::pair<float, float> Self::getSize() const {
    auto&& [width, height] = ad_->getSize();
    return std::pair(width / metrics_->toPixel(), height / metrics_->toPixel());
}

void Self::setSize(float width, float height) {
    ad_->setSize(metrics_->toPixel() * width, metrics_->toPixel() * height);
}

bool Self::isVisible() const {
    return ad_->isVisible();
}

void Self::setVisible(bool visible) {
    ad_->setVisible(visible);
}
} // namespace services
} // namespace ee

#endif // EE_X_COCOS_CPP
