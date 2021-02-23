//
//  IronSourceInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#ifndef EE_X_IRON_SOURCE_INTERSTITIAL_AD_HPP
#define EE_X_IRON_SOURCE_INTERSTITIAL_AD_HPP

#include <string>

#include <ee/ads/IFullScreenAd.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/iron_source/IronSourceFwd.hpp"

namespace ee {
namespace iron_source {
class InterstitialAd : public IFullScreenAd,
                       public ObserverManager<AdObserver> {
public:
    explicit InterstitialAd(
        ILogger& logger,
        const std::shared_ptr<ads::IAsyncHelper<AdResult>>& displayer,
        Bridge* plugin, const std::string& adId);
    virtual ~InterstitialAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<AdResult> show() override;

private:
    friend Bridge;

    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onFailedToShow(const std::string& message);
    void onClicked();
    void onClosed();

    ILogger& logger_;
    std::shared_ptr<ads::IAsyncHelper<AdResult>> displayer_;
    Bridge* plugin_;
    std::string adId_;

    std::unique_ptr<ads::IAsyncHelper<bool>> loader_;
};
} // namespace iron_source
} // namespace ee

#endif /* EE_X_IRON_SOURCE_INTERSTITIAL_AD_HPP */
