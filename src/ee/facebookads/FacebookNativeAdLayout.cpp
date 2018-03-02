//
//  FacebookNativeAdBuilder.cpp
//  ee_x
//
//  Created by Zinge on 10/9/17.
//
//

#include "ee/facebookads/FacebookNativeAdLayout.hpp"

namespace ee {
namespace facebook {
using Self = NativeAdLayout;

namespace {
// clang-format off
constexpr auto k__ad_choices     = "ad_choices";
constexpr auto k__body           = "body";
constexpr auto k__call_to_action = "call_to_action";
constexpr auto k__icon           = "icon";
constexpr auto k__media          = "media";
constexpr auto k__social_context = "social_context";
constexpr auto k__title          = "title";
// clang-format on
} // namespace

Self::NativeAdLayout() {}

Self::~NativeAdLayout() {}

Self& Self::setAdChoices(const std::string& id) {
    params_[k__ad_choices] = id;
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

Self& Self::setMedia(const std::string& id) {
    params_[k__media] = id;
    return *this;
}

Self& Self::setSocialContext(const std::string& id) {
    params_[k__social_context] = id;
    return *this;
}

Self& Self::setTitle(const std::string& id) {
    params_[k__title] = id;
    return *this;
}
} // namespace facebook
} // namespace ee
