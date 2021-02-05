//
//  AdMobFwd.hpp
//  ee_x
//
//  Created by Zinge on 10/16/17.
//
//

#ifndef EE_X_AD_MOB_FWD_HPP
#define EE_X_AD_MOB_FWD_HPP

#ifdef __cplusplus

#include <ee/ads/AdsFwd.hpp>

namespace ee {
namespace ad_mob {
enum class BannerAdSize;

class IBridge;
class Bridge;
class NativeAdLayout;
} // namespace ad_mob

using IAdMob = ad_mob::IBridge;
using AdMobBannerAdSize = ad_mob::BannerAdSize;
using AdMobNativeAdLayout = ad_mob::NativeAdLayout;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_AD_MOB_FWD_HPP */
