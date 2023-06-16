#ifndef EE_X_APP_LOVIN_MAX_INTERSTITIAL_AD_HPP
#define EE_X_APP_LOVIN_MAX_INTERSTITIAL_AD_HPP

#include <string>

#include <ee/ads/IFullScreenAd.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/app_lovin_max/AppLovinMaxFwd.hpp"

namespace ee {
namespace app_lovin_max {
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
    void onFailedToLoad(int code, const std::string& message);
    void onFailedToShow(int code, const std::string& message);
    void onClicked();
    void onClosed();
    void onAdPaid(const ads::AdPaidResult& result);

    ILogger& logger_;
    std::shared_ptr<ads::IAsyncHelper<AdResult>> displayer_;
    Bridge* plugin_;
    std::string adId_;

    std::unique_ptr<ads::IAsyncHelper<bool>> loader_;
};
} // namespace app_lovin_max
} // namespace ee

#endif /* EE_X_APP_LOVIN_MAX_INTERSTITIAL_AD_HPP */
