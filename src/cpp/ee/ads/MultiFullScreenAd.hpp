#ifndef EE_X_MULTI_FULL_SCREEN_AD_HPP
#define EE_X_MULTI_FULL_SCREEN_AD_HPP

#ifdef __cplusplus

#include <memory>
#include <vector>

#include <ee/core/ObserverManager.hpp>

#include "ee/ads/IFullScreenAd.hpp"

namespace ee {
namespace ads {
class MultiFullScreenAd : public IFullScreenAd,
                          public ObserverManager<AdObserver> {
private:
    using Self = MultiFullScreenAd;

public:
    MultiFullScreenAd();
    virtual ~MultiFullScreenAd() override;

    Self& addItem(const std::shared_ptr<IFullScreenAd>& item);

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<FullScreenAdResult> show() override;

private:
    std::vector<std::shared_ptr<IFullScreenAd>> items_;
    std::unique_ptr<ObserverHandle> handle_;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif // EE_X_MULTI_FULL_SCREEN_AD_HPP
