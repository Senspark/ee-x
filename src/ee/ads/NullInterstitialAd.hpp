//
//  NullInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/27/17.
//
//

#ifndef EE_X_NULL_INTERSTITIAL_AD_HPP
#define EE_X_NULL_INTERSTITIAL_AD_HPP

#include <ee/core/ObserverManager.hpp>

#include "ee/ads/IInterstitialAd.hpp"

namespace ee {
namespace ads {
class NullInterstitialAd : public IInterstitialAd,
                           public ObserverManager<IInterstitialAdObserver> {
public:
    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<bool> show() override;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_NULL_INTERSTITIAL_AD_HPP */
