//
//  RewardedVideoList.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#ifndef EE_X_MULTI_REWARDED_AD_HPP
#define EE_X_MULTI_REWARDED_AD_HPP

#ifdef __cplusplus

#include <memory>
#include <vector>

#include <ee/core/SafeObserverManager.hpp>

#include "ee/ads/IRewardedAd.hpp"

namespace ee {
namespace ads {
class MultiRewardedAd : public IRewardedAd,
                        public SafeObserverManager<IRewardedAdObserver> {
private:
    using Self = MultiRewardedAd;

public:
    MultiRewardedAd();
    virtual ~MultiRewardedAd() override;

    Self& addItem(const std::shared_ptr<IRewardedAd>& item);

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<IRewardedAdResult> show() override;

private:
    std::vector<std::shared_ptr<IRewardedAd>> items_;
    std::unique_ptr<ObserverHandle> handle_;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_MULTI_REWARDED_AD_HPP */
