//
//  AdMobRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/13/17.
//
//

#ifndef EE_X_ADMOB_REWARDED_AD_HPP
#define EE_X_ADMOB_REWARDED_AD_HPP

#include <string>

#include <ee/ads/IRewardedAd.hpp>
#include <ee/ads/internal/MessageHelper.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/AdMobFwd.hpp"

namespace ee {
namespace admob {
class RewardedAd final : public IRewardedAd,
                         public ObserverManager<IRewardedAdObserver> {
public:
    virtual ~RewardedAd() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<IRewardedAdResult> show() override;

private:
    friend Bridge;

    explicit RewardedAd(IMessageBridge& bridge, const Logger& logger,
                        Bridge* plugin, const std::string& adId);

    bool createInternalAd();
    bool destroyInternalAd();

    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onFailedToShow(const std::string& message);
    void onClosed(bool rewarded);

    IMessageBridge& bridge_;
    const Logger& logger_;
    Bridge* plugin_;
    std::string adId_;
    ads::MessageHelper messageHelper_;

    std::unique_ptr<ads::AsyncHelper<bool>> loader_;
    std::unique_ptr<ads::AsyncHelper<IRewardedAdResult>> displayer_;
};
} // namespace admob
} // namespace ee

#endif /* EE_X_ADMOB_REWARDED_AD_HPP */
