#include "ee/ads/internal/MessageHelper.hpp"

namespace ee {
namespace ads {
using Self = MessageHelper;

Self::MessageHelper(const std::string& prefix, const std::string& adId)
    : prefix_(prefix)
    , adId_(adId) {}

std::string Self::isLoaded() const {
    return prefix_ + "_isLoaded_" + adId_;
}

std::string Self::load() const {
    return prefix_ + "_load_" + adId_;
}

std::string Self::onLoaded() const {
    return prefix_ + "_onLoaded_" + adId_;
}

std::string Self::onFailedToLoad() const {
    return prefix_ + "_onFailedToLoad_" + adId_;
}

std::string Self::show() const {
    return prefix_ + "_show_" + adId_;
}

std::string Self::onFailedToShow() const {
    return prefix_ + "_onFailedToShow_" + adId_;
}

std::string Self::onClicked() const {
    return prefix_ + "_onClicked_" + adId_;
}

std::string Self::onClosed() const {
    return prefix_ + "_onClosed_" + adId_;
}

std::string Self::getPosition() const {
    return prefix_ + "_getPosition_" + adId_;
}

std::string Self::setPosition() const {
    return prefix_ + "_setPosition_" + adId_;
}

std::string Self::getSize() const {
    return prefix_ + "_getSize_" + adId_;
}

std::string Self::setSize() const {
    return prefix_ + "_setSize_" + adId_;
}

std::string Self::isVisible() const {
    return prefix_ + "_isVisible_" + adId_;
}

std::string Self::setVisible() const {
    return prefix_ + "_setVisible_" + adId_;
}

std::string Self::createInternalAd() const {
    return prefix_ + "_createInternalAd_" + adId_;
}

std::string Self::destroyInternalAd() const {
    return prefix_ + "_destroyInternalAd_" + adId_;
}
} // namespace ads
} // namespace ee