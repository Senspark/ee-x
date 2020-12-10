//
//  FacebookBannerAdSize.hpp
//  ee_x
//
//  Created by Zinge on 10/13/17.
//
//

#ifndef EE_X_FACEBOOK_BANNER_AD_SIZE_HPP
#define EE_X_FACEBOOK_BANNER_AD_SIZE_HPP

#ifdef __cplusplus

#include "ee/FacebookAdsFwd.hpp"

namespace ee {
namespace facebook_ads {
enum class BannerAdSize {
    /// kFBAdSizeHeight50Banner or AdSize.BANNER_HEIGHT_50.
    BannerHeight50,

    /// kFBAdSizeHeight90Banner or AdSize.BANNER_HEIGHT_90.
    BannerHeight90,

    /// kFBAdSizeInterstitial or AdSize.INTERSTITIAL.
    Interstitial,

    /// kFBAdSizeHeight250Rectangle or AdSize.RECTANGLE_HEIGHT_250.
    RectangleHeight250,

    /// kFBAdSize320x50
    Size320x50,
};
} // namespace facebook_ads
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_FACEBOOK_BANNER_AD_SIZE_HPP */
