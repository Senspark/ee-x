//
//  FacebookBannerAdSize.hpp
//  ee_x
//
//  Created by Zinge on 10/13/17.
//
//

#ifndef EE_X_FACEBOOK_BANNER_AD_SIZE_HPP
#define EE_X_FACEBOOK_BANNER_AD_SIZE_HPP

namespace ee {
namespace facebook {
enum class BannerAdSize {
    /// kFBAdSizeHeight50Banner or AdSize.BANNER_HEIGHT_50.
    BannerHeight50,

    /// kFBAdSizeHeight90Banner or AdSize.BANNER_HEIGHT_90.
    BannerHeight90,

    /// kFBAdSizeInterstitial or AdSize.INTERSTITIAL.
    Interstitial,

    /// kFBAdSizeHeight250Rectangle or AdSize.RECTANGLE_HEIGHT_250.
    RectangleHeight250,
};
} // namespace facebook

using FacebookBannerAdSize = facebook::BannerAdSize;
} // namespace ee

#endif /* EE_X_FACEBOOK_BANNER_AD_SIZE_HPP */
