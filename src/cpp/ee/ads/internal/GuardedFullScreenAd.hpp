#ifndef EE_X_GUARDED_FULL_SCREEN_AD_HPP
#define EE_X_GUARDED_FULL_SCREEN_AD_HPP

#ifdef __cplusplus

#include <ee/core/ObserverManager.hpp>

#include "ee/ads/IFullScreenAd.hpp"

namespace ee {
namespace ads {
class GuardedFullScreenAd : public IFullScreenAd,
                            public ObserverManager<AdObserver> {
public:
    explicit GuardedFullScreenAd(const std::shared_ptr<IFullScreenAd>& ad);
    virtual ~GuardedFullScreenAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<AdResult> show() override;

private:
    std::shared_ptr<IFullScreenAd> ad_;

    bool loading_;
    bool loaded_;
    bool displaying_;
    std::unique_ptr<ObserverHandle> handle_;
    std::unique_ptr<Retrier> retrier_;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif // EE_X_GUARDED_FULL_SCREEN_AD_HPP
