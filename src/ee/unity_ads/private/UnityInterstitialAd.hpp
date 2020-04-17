//
//  UnityInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#ifndef EE_X_UNITY_INTERSTITIAL_AD_HPP
#define EE_X_UNITY_INTERSTITIAL_AD_HPP

#include <string>

#include <ee/ads/IInterstitialAd.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/UnityAdsFwd.hpp"

namespace ee {
namespace unity_ads {
class InterstitialAd : public IInterstitialAd,
                       public ee::ObserverManager<IInterstitialAdObserver> {
public:
    virtual ~InterstitialAd() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<bool> show() override;

private:
    friend Bridge;

    explicit InterstitialAd(const Logger& logger, Bridge* plugin,
                            const std::string& adId);

    void onFailedToShow(const std::string& message);
    void onClosed();

    const Logger& logger_;
    Bridge* plugin_;
    std::string adId_;

    std::unique_ptr<ads::AsyncHelper<bool>> displayer_;
};
} // namespace unity_ads
} // namespace ee

#endif /* EE_X_UNITY_INTERSTITIAL_AD_HPP */
