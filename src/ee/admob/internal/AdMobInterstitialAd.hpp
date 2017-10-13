//
//  AdMobInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#ifndef EE_X_ADMOB_INTERSTITIAL_AD_HPP
#define EE_X_ADMOB_INTERSTITIAL_AD_HPP

#include "ee/ads/InterstitialAdInterface.hpp"

namespace ee {
namespace admob {
class AdMob;

class InterstitialAd : public InterstitialAdInterface {
public:
    virtual ~InterstitialAd() override;

    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;

protected:
    explicit InterstitialAd(AdMob* plugin, const std::string& adId);

    bool createInternalAd();
    bool destroyInternalAd();

    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onFailedToShow();
    void onClosed();

private:
    friend AdMob;

    /// Whether the ad is in loading progress.
    bool loading_;

    bool errored_;

    AdMob* plugin_;
    std::string adId_;
};
}; // namespace admob
} // namespace ee

#endif /* EE_X_ADMOB_INTERSTITIAL_AD_HPP */
