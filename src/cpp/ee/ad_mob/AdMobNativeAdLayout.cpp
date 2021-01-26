//
//  AdMobNativeAdBuilder.cpp
//  ee_x
//
//  Created by Zinge on 10/16/17.
//
//

#include "ee/ad_mob/AdMobNativeAdLayout.hpp"

namespace ee {
namespace ad_mob {
namespace {
// clang-format off
constexpr auto k__body           = "body";
constexpr auto k__call_to_action = "call_to_action";
constexpr auto k__headline       = "headline";
constexpr auto k__icon           = "icon";
constexpr auto k__image          = "image";
constexpr auto k__media          = "media";
constexpr auto k__price          = "price";
constexpr auto k__star_rating    = "star_rating";
constexpr auto k__store          = "store";
// clang-format on
} // namespace

using Self = NativeAdLayout;

Self::NativeAdLayout() = default;
Self::~NativeAdLayout() = default;

Self& Self::setBody(const std::string& id) {
    params_[k__body] = id;
    return *this;
}

Self& Self::setCallToAction(const std::string& id) {
    params_[k__call_to_action] = id;
    return *this;
}

Self& Self::setHeadline(const std::string& id) {
    params_[k__headline] = id;
    return *this;
}

Self& Self::setIcon(const std::string& id) {
    params_[k__icon] = id;
    return *this;
}

Self& Self::setImage(const std::string& id) {
    params_[k__image] = id;
    return *this;
}

Self& Self::setMedia(const std::string& id) {
    params_[k__media] = id;
    return *this;
}

Self& Self::setPrice(const std::string& id) {
    params_[k__price] = id;
    return *this;
}

Self& Self::setStarRating(const std::string& id) {
    params_[k__star_rating] = id;
    return *this;
}

Self& Self::setStore(const std::string& id) {
    params_[k__store] = id;
    return *this;
}
} // namespace ad_mob
} // namespace ee
