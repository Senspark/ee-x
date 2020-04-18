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

#include <ee/ads/IInterstitialAd.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/IronSourceFwd.hpp"

namespace ee {
namespace iron_source {
class InterstitialAd : public IInterstitialAd,
                       public ObserverManager<IInterstitialAdObserver> {
public:
    virtual ~InterstitialAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<bool> show() override;

private:
    friend Bridge;

    explicit InterstitialAd(
        const Logger& logger,
        const std::shared_ptr<ads::IAsyncHelper<bool>>& displayer,
        Bridge* plugin, const std::string& adId);

    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onFailedToShow(const std::string& message);
    void onClicked();
    void onClosed();

    const Logger& logger_;
    std::shared_ptr<ads::IAsyncHelper<bool>> displayer_;
    Bridge* plugin_;
    std::string adId_;

    std::unique_ptr<ads::IAsyncHelper<bool>> loader_;
};
} // namespace iron_source
} // namespace ee

#endif /* EE_X_IRON_SOURCE_INTERSTITIAL_AD_HPP */
