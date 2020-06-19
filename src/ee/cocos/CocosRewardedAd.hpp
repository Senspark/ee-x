//
//  CocosRewardedAd.hpp
//  Pods
//
//  Created by eps on 6/19/20.
//

#ifndef EE_X_COCOS_REWARDED_AD_HPP
#define EE_X_COCOS_REWARDED_AD_HPP

#ifdef __cplusplus

#include <ee/ads/IRewardedAd.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/CocosFwd.hpp"

namespace ee {
namespace cocos {
class CocosRewardedAd : public IRewardedAd,
                        public ObserverManager<IRewardedAdObserver> {
private:
    using Self = CocosRewardedAd;
    using Super = IRewardedAd;

public:
    CocosRewardedAd(const std::shared_ptr<IRewardedAd>& ad);
    virtual ~CocosRewardedAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<IRewardedAdResult> show() override;

private:
    std::shared_ptr<IRewardedAd> ad_;
    std::unique_ptr<ObserverHandle> handle_;
};
} // namespace cocos
} // namespace ee

#endif __cplusplus

#endif /* EE_X_COCOS_REWARDED_AD_HPP */
