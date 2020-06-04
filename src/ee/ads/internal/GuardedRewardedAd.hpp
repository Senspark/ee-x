#ifndef EE_X_GUARDED_REWARDED_AD_HPP
#define EE_X_GUARDED_REWARDED_AD_HPP

#ifdef __cplusplus

#include <ee/core/SafeObserverManager.hpp>

#include "ee/ads/IRewardedAd.hpp"

namespace ee {
namespace ads {
class GuardedRewardedAd : public IRewardedAd,
                          public SafeObserverManager<IRewardedAdObserver> {
public:
    explicit GuardedRewardedAd(const std::shared_ptr<IRewardedAd>& ad);
    virtual ~GuardedRewardedAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<IRewardedAdResult> show() override;

private:
    std::shared_ptr<IRewardedAd> ad_;

    bool loading_;
    bool loaded_;
    bool displaying_;
    std::unique_ptr<ObserverHandle> handle_;
    std::unique_ptr<SpinLock> lock_;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif // EE_X_GUARDED_REWARDED_AD_HPP
