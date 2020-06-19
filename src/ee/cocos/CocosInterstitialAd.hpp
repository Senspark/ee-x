//
//  CocosInterstitialAd.hpp
//  Pods
//
//  Created by eps on 6/18/20.
//

#ifndef EE_X_COCOS_INTERSTITIAL_AD_HPP
#define EE_X_COCOS_INTERSTITIAL_AD_HPP

#ifdef __cplusplus

#include <ee/ads/IInterstitialAd.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/CocosFwd.hpp"

namespace ee {
namespace cocos {
class CocosInterstitialAd : public IInterstitialAd,
                            public ObserverManager<IInterstitialAdObserver> {
private:
    using Self = CocosInterstitialAd;
    using Super = IInterstitialAd;

public:
    CocosInterstitialAd(const std::shared_ptr<IInterstitialAd>& ad);
    virtual ~CocosInterstitialAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<bool> show() override;

private:
    std::shared_ptr<IInterstitialAd> ad_;
    std::unique_ptr<ObserverHandle> handle_;
};
} // namespace cocos
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_COCOS_INTERSTITIAL_AD_HPP */
