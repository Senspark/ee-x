#ifndef EE_X_GUARDED_INTERSTITIAL_AD_HPP
#define EE_X_GUARDED_INTERSTITIAL_AD_HPP

#include <ee/core/ObserverManager.hpp>

#include <ee/ads/IInterstitialAd.hpp>

namespace ee {
namespace ads {
class GuardedInterstitialAd : public IInterstitialAd,
                              public ObserverManager<IInterstitialAdObserver> {
public:
    explicit GuardedInterstitialAd(const std::shared_ptr<IInterstitialAd>& ad);
    virtual ~GuardedInterstitialAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<bool> show() override;

private:
    std::shared_ptr<IInterstitialAd> ad_;

    bool loading_;
    bool loaded_;
    bool displaying_;
    std::unique_ptr<ObserverHandle> handle_;
};
} // namespace ads
} // namespace ee

#endif // EE_X_GUARDED_INTERSTITIAL_AD_HPP