//
//  CocosAdView.cpp
//  Pods
//
//  Created by eps on 6/18/20.
//

#include "ee/cocos/CocosAdView.hpp"

#include <base/CCDirector.h>

#include <ee/core/ObserverHandle.hpp>
#include <ee/core/Task.hpp>

#include "ee/cocos/Metrics.hpp"

namespace ee {
namespace cocos {
using Self = CocosAdView;

Self::CocosAdView(const std::shared_ptr<IAdView>& ad)
    : ad_(ad) {
    handle_ = std::make_unique<ObserverHandle>();
    handle_->bind(*ad_).addObserver({
        .onLoaded =
            [this] {
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
    ad_->setAnchor(0.0f, 1.0f); // Reserved anchor.
    metrics_ = std::make_unique<Metrics>(Metrics::fromPoint(1));
    sceneHeight_ = cocos2d::Director::getInstance()->getWinSize().height;
}

Self::~CocosAdView() = default;

void Self::destroy() {
    ad_->destroy();
    handle_->clear();
}

bool Self::isLoaded() const {
    return ad_->isLoaded();
}

Task<bool> Self::load() {
    auto result = co_await ad_->load();
    co_return result;
}

std::pair<float, float> Self::getAnchor() const {
    return ad_->getAnchor();
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
} // namespace cocos
} // namespace ee
