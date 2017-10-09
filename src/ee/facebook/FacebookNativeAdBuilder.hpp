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

namespace ee {
namespace facebook {
class FacebookAds;

class FacebookNativeAdBuilder final {
private:
    using Self = FacebookNativeAdBuilder;

public:
    FacebookNativeAdBuilder();
    ~FacebookNativeAdBuilder();

    /// Sets the native ad ID.
    Self& setAdId(const std::string& adId);

    /// Sets the xib (for iOS) or xml (for Android) layout name.
    Self& setLayoutName(const std::string& layoutName);

    /// Sets the icon label resource name.
    /// Only effective on Android.
    Self& setIcon(const std::string& key);

    /// Only effective on Android.
    Self& setTitle(const std::string& key);

    /// Only effective on Android.
    Self& setMedia(const std::string& key);

    /// Only effective on Android.
    Self& setSocialContext(const std::string& key);

    /// Only effective on Android.
    Self& setAdChoices(const std::string& key);

    /// Only effective on Android.
    Self& setBody(const std::string& key);

    /// Only effective on Android.
    Self& setAction(const std::string& key);

private:
    friend FacebookAds;

    std::string adId_;
    std::string layoutName_;
    std::string icon_;
    std::string title_;
    std::string media_;
    std::string socialContext_;
    std::string adChoices_;
    std::string body_;
    std::string action_;
};
} // namespace facebook

using facebook::FacebookNativeAdBuilder;
} // namespace ee

#endif /* EE_X_FACEBOOK_NATIVE_AD_BUILDER_HPP */
