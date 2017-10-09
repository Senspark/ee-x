//
//  FacebookNativeAdBuilder.cpp
//  ee_x
//
//  Created by Zinge on 10/9/17.
//
//

#include "ee/facebook/FacebookNativeAdBuilder.hpp"

namespace ee {
namespace facebook {
using Self = FacebookNativeAdBuilder;

Self::FacebookNativeAdBuilder() {}

Self::~FacebookNativeAdBuilder() {}

Self& Self::setAdId(const std::string& adId) {
    adId_ = adId;
    return *this;
}

Self& Self::setLayoutName(const std::string& layoutName) {
    layoutName_ = layoutName;
    return *this;
}

Self& Self::setIcon(const std::string& key) {
    icon_ = key;
    return *this;
}

Self& Self::setTitle(const std::string& key) {
    title_ = key;
    return *this;
}

Self& Self::setMedia(const std::string& key) {
    media_ = key;
    return *this;
}

Self& Self::setSocialContext(const std::string& key) {
    socialContext_ = key;
    return *this;
}

Self& Self::setAdChoices(const std::string& key) {
    adChoices_ = key;
    return *this;
}

Self& Self::setBody(const std::string& key) {
    body_ = key;
    return *this;
}

Self& Self::setAction(const std::string& key) {
    action_ = key;
    return *this;
}
} // namespace facebook
} // namespace ee
