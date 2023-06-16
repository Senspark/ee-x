#ifndef EE_X_APP_LOVIN_MAX_REWARDED_AD_HPP
#define EE_X_APP_LOVIN_MAX_REWARDED_AD_HPP

#include <ee/ads/IFullScreenAd.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/app_lovin_max/AppLovinMaxFwd.hpp"

namespace ee {
namespace app_lovin_max {
class RewardedAd : public IFullScreenAd, public ObserverManager<AdObserver> {
public:
    explicit RewardedAd(
        ILogger& logger,
        const std::shared_ptr<ads::IAsyncHelper<AdResult>>& displayer,
        Bridge* plugin, const std::string& adId);
    virtual ~RewardedAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<AdResult> show() override;

private:
    friend Bridge;

    void onLoaded();
    void onFailedToShow(int code, const std::string& message);
    void onClicked();
    void onClosed(bool rewarded);
    void onAdPaid(const ads::AdPaidResult& result);

    ILogger& logger_;
    std::shared_ptr<ads::IAsyncHelper<AdResult>> displayer_;
    Bridge* plugin_;
    std::string adId_;
};
} // namespace app_lovin_max
} // namespace ee

#endif /* EE_X_APP_LOVIN_MAX_REWARDED_AD_HPP */
