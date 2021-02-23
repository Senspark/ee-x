//
//  VungleRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#ifndef EE_X_VUNGLE_REWARDED_AD_HPP
#define EE_X_VUNGLE_REWARDED_AD_HPP

#include <string>

#include <ee/ads/IFullScreenAd.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/vungle/VungleFwd.hpp"

namespace ee {
namespace vungle {
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
    void onFailedToLoad(const std::string& message);
    void onFailedToShow(const std::string& message);
    void onClicked();
    void onClosed(bool rewarded);

    ILogger& logger_;
    std::shared_ptr<ads::IAsyncHelper<AdResult>> displayer_;
    Bridge* plugin_;
    std::string adId_;

    std::unique_ptr<ads::IAsyncHelper<bool>> loader_;
};
} // namespace vungle
} // namespace ee

#endif /* EE_X_VUNGLE_REWARDED_AD_HPP */
