//
//  FacebookInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#ifndef EE_X_FACEBOOK_INTERSTITIAL_AD_HPP
#define EE_X_FACEBOOK_INTERSTITIAL_AD_HPP

#include <string>

#include <ee/ads/IInterstitialAd.hpp>

#include "ee/FacebookAdsFwd.hpp"

namespace ee {
namespace facebook_ads {
class InterstitialAd : public IInterstitialAd {
public:
    virtual ~InterstitialAd() override;

    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;

protected:
    explicit InterstitialAd(IMessageBridge& bridge, const Logger& logger,
                            Bridge* plugin,
                            const std::string& placementId);

    bool createInternalAd();
    bool destroyInternalAd();

    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onClosed();
    void onClicked();

private:
    friend Bridge;

    /// Whether the ad is in loading progress.
    bool loading_;

    IMessageBridge& bridge_;
    const Logger& logger_;
    Bridge* plugin_;
    std::string placementId_;
};
} // namespace facebook_ads
} // namespace ee

#endif /* EE_X_FACEBOOK_INTERSTITIAL_AD_HPP */
