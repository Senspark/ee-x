//
//  FacebookInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#ifndef EE_X_FACEBOOK_INTERSTITIAL_AD_HPP
#define EE_X_FACEBOOK_INTERSTITIAL_AD_HPP

#include "ee/FacebookAdsFwd.hpp"
#include "ee/ads/IInterstitialAd.hpp"

namespace ee {
namespace facebook {
class InterstitialAd : public IInterstitialAd {
public:
    virtual ~InterstitialAd() override;

    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;

protected:
    explicit InterstitialAd(IMessageBridge& bridge, Logger& logger,
                            FacebookAds* plugin,
                            const std::string& placementId);

    bool createInternalAd();
    bool destroyInternalAd();

    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onClosed();

private:
    friend FacebookAds;

    /// Whether the ad is in loading progress.
    bool loading_;

    IMessageBridge& bridge_;
    Logger& logger_;
    FacebookAds* plugin_;
    std::string placementId_;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_INTERSTITIAL_AD_HPP */
