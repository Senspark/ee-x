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

#include <ee/ads/IFullScreenAd.hpp>
#include <ee/ads/internal/MessageHelper.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/facebook_ads/FacebookAdsFwd.hpp"

namespace ee {
namespace facebook_ads {
class InterstitialAd : public IFullScreenAd,
                       public ObserverManager<AdObserver> {
public:
    explicit InterstitialAd(
        IMessageBridge& bridge, const Logger& logger,
        const std::shared_ptr<ads::IAsyncHelper<FullScreenAdResult>>& displayer,
        Bridge* plugin, const std::string& adId);
    virtual ~InterstitialAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<FullScreenAdResult> show() override;

private:
    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onFailedToShow(const std::string& message);
    void onClosed();
    void onClicked();

    IMessageBridge& bridge_;
    const Logger& logger_;
    std::shared_ptr<ads::IAsyncHelper<FullScreenAdResult>> displayer_;
    Bridge* plugin_;
    std::string adId_;
    ads::MessageHelper messageHelper_;

    std::unique_ptr<ads::IAsyncHelper<bool>> loader_;
};
} // namespace facebook_ads
} // namespace ee

#endif /* EE_X_FACEBOOK_INTERSTITIAL_AD_HPP */
