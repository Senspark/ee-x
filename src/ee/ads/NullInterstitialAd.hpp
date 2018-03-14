//
//  NullInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/27/17.
//
//

#ifndef EE_X_NULL_INTERSTITIAL_AD_HPP
#define EE_X_NULL_INTERSTITIAL_AD_HPP

#include "ee/ads/IInterstitialAd.hpp"

namespace ee {
namespace ads {
class NullInterstitialAd : public IInterstitialAd {
public:
    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_NULL_INTERSTITIAL_AD_HPP */
