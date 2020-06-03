//
//  MultiInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#ifndef EE_X_MULTI_INTERSTITIAL_AD_HPP
#define EE_X_MULTI_INTERSTITIAL_AD_HPP

#ifdef __cplusplus

#include <memory>
#include <vector>

#include <ee/core/SafeObserverManager.hpp>

#include "ee/ads/IInterstitialAd.hpp"

namespace ee {
namespace ads {
class MultiInterstitialAd
    : public IInterstitialAd,
      public SafeObserverManager<IInterstitialAdObserver> {
private:
    using Self = MultiInterstitialAd;
    using Super = IInterstitialAd;

public:
    MultiInterstitialAd();
    virtual ~MultiInterstitialAd() override;

    Self& addItem(const std::shared_ptr<IInterstitialAd>& item);

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<bool> show() override;

private:
    std::vector<std::shared_ptr<IInterstitialAd>> items_;
    std::unique_ptr<ObserverHandle> handle_;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_MULTI_INTERSTITIAL_AD_HPP */
