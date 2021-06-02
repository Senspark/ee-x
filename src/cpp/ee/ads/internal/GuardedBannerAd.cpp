#include "ee/ads/internal/GuardedBannerAd.hpp"

#include <ee/core/ObserverHandle.hpp>
#include <ee/core/Task.hpp>

namespace ee {
namespace ads {
using Self = GuardedBannerAd;

Self::GuardedBannerAd(const std::shared_ptr<IBannerAd>& ad,
                      const std::shared_ptr<ICapper>& capper,
                      const std::shared_ptr<IRetrier>& retrier)
    : GuardedAd(ad, capper, retrier)
    , ad_(ad) {}

Self::~GuardedBannerAd() = default;

bool Self::isDisplaying() const {
    return false;
}

std::pair<float, float> Self::getAnchor() const {
    return ad_->getAnchor();
}

void Self::setAnchor(float x, float y) {
    ad_->setAnchor(x, y);
}

std::pair<float, float> Self::getPosition() const {
    return ad_->getPosition();
}

void Self::setPosition(float x, float y) {
    ad_->setPosition(x, y);
}

std::pair<float, float> Self::getSize() const {
    return ad_->getSize();
}

void Self::setSize(float width, float height) {
    ad_->setSize(width, height);
}

bool Self::isVisible() const {
    return ad_->isVisible();
}

void Self::setVisible(bool visible) {
    ad_->setVisible(visible);
}
} // namespace ads
} // namespace ee
