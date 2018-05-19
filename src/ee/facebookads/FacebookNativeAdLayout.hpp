//
//  FacebookNativeAdBuilder.hpp
//  ee_x
//
//  Created by Zinge on 10/9/17.
//
//

#ifndef EE_X_FACEBOOK_NATIVE_AD_BUILDER_HPP
#define EE_X_FACEBOOK_NATIVE_AD_BUILDER_HPP

#include <string>
#include <unordered_map>

#include "ee/FacebookAdsFwd.hpp"

namespace ee {
namespace facebook {
class NativeAdLayout final {
private:
    using Self = NativeAdLayout;

public:
    NativeAdLayout();
    ~NativeAdLayout();
    
    Self& setAdChoices(const std::string& id);
    Self& setBody(const std::string& id);
    Self& setCallToAction(const std::string& id);
    Self& setIcon(const std::string& id);
    Self& setMedia(const std::string& id);
    Self& setSocialContext(const std::string& id);
    Self& setTitle(const std::string& id);
    Self& setCover(const std::string& id);
    Self& setSponsor(const std::string& id);
private:
    friend FacebookAds;

    std::unordered_map<std::string, std::string> params_;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_NATIVE_AD_BUILDER_HPP */
