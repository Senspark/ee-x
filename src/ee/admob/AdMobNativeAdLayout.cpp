//
//  AdMobNativeAdBuilder.cpp
//  ee_x
//
//  Created by Zinge on 10/16/17.
//
//

#include "ee/admob/AdMobNativeAdLayout.hpp"

namespace ee {
namespace admob {
namespace {
// clang-format off
constexpr auto k__headline       = "headline";
constexpr auto k__body           = "body";
constexpr auto k__call_to_action = "call_to_action";
constexpr auto k__icon           = "icon";
constexpr auto k__price          = "price";
constexpr auto k__star_rating    = "star_rating";
constexpr auto k__media          = "media";
constexpr auto k__main_image     = "main_image";
// clang-format on
} // namespace

using Self = NativeAdLayout;

Self::NativeAdLayout() {}
Self::~NativeAdLayout() {}

Self& Self::setHeadline(const std::string& id) {
    params_[k__headline] = id;
    return *this;
}

Self& Self::setBody(const std::string& id) {
    params_[k__body] = id;
    return *this;
}

Self& Self::setCallToAction(const std::string& id) {
    params_[k__call_to_action] = id;
    return *this;
}

Self& Self::setIcon(const std::string& id) {
    params_[k__icon] = id;
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

Self& Self::setMedia(const std::string& id) {
    params_[k__media] = id;
    return *this;
}

Self& Self::setMainImage(const std::string& id) {
    params_[k__main_image] = id;
    return *this;
}
} // namespace admob
} // namespace ee
