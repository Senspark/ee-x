//
//  UnityInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#ifndef EE_X_UNITY_INTERSTITIAL_AD_HPP
#define EE_X_UNITY_INTERSTITIAL_AD_HPP

#include "ee/UnityAdsFwd.hpp"
#include "ee/ads/IInterstitialAd.hpp"

namespace ee {
namespace unityads {
class InterstitialAd : public IInterstitialAd {
public:
    virtual ~InterstitialAd() override;

    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;

protected:
    explicit InterstitialAd(UnityAds* plugin, const std::string& placementId);

private:
    friend UnityAds;

    UnityAds* plugin_;
    std::string placementId_;
};
} // namespace unityads
} // namespace ee

#endif /* EE_X_UNITY_INTERSTITIAL_AD_HPP */
