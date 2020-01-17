//
//  AdMobInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#ifndef EE_X_ADMOB_INTERSTITIAL_AD_HPP
#define EE_X_ADMOB_INTERSTITIAL_AD_HPP

#include <string>

#include <ee/ads/IInterstitialAd.hpp>

#include "ee/AdMobFwd.hpp"

namespace ee {
namespace admob {
class InterstitialAd : public IInterstitialAd {
public:
    virtual ~InterstitialAd() override;

    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;

protected:
    explicit InterstitialAd(IMessageBridge& bridge, const Logger& logger,
                            AdMob* plugin, const std::string& adId);

    bool createInternalAd();
    bool destroyInternalAd();

    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onFailedToShow();
    void onClosed();
    void onClicked();

private:
    friend AdMob;

    /// Whether the ad is in loading progress.
    bool loading_;

    bool errored_;

    IMessageBridge& bridge_;
    const Logger& logger_;
    AdMob* plugin_;
    std::string adId_;
};
}; // namespace admob
} // namespace ee

#endif /* EE_X_ADMOB_INTERSTITIAL_AD_HPP */
