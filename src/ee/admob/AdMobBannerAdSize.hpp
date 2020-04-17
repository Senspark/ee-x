//
//  AdMobBannerAdSize.hpp
//  ee_x
//
//  Created by Zinge on 10/13/17.
//
//

#ifndef EE_X_ADMOB_BANNER_AD_SIZE_HPP
#define EE_X_ADMOB_BANNER_AD_SIZE_HPP

#include "ee/AdMobFwd.hpp"

namespace ee {
namespace admob {
enum class BannerAdSize {
    /// kGADAdSizeBanner
    /// Typically 320x50.
    Normal,

    /// kGADAdSizeLargeBanner
    /// Typically 320x100.
    Large,

    /// kGADAdSizeSmartBannerPortrait or kGADAdSizeSmartBannerLandscape
    /// Spans the full width of the application.
    Smart,

    /// kGADAdSizeMediumRectangle.
    /// Typically 300x250.
    MediumReactangle,
};
} // namespace admob
} // namespace ee

#endif /* EE_X_ADMOB_BANNER_AD_SIZE_HPP */
