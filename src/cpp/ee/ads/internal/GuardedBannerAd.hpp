#ifndef EE_X_GUARDED_BANNER_AD_HPP
#define EE_X_GUARDED_BANNER_AD_HPP

#ifdef __cplusplus

#include <ee/core/ObserverManager.hpp>

#include "ee/ads/IBannerAd.hpp"

namespace ee {
namespace ads {
class GuardedBannerAd : public IBannerAd, public ObserverManager<AdObserver> {
public:
    explicit GuardedBannerAd(const std::shared_ptr<IBannerAd>& ad);

    virtual ~GuardedBannerAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;

    virtual std::pair<float, float> getAnchor() const override;
    virtual void setAnchor(float x, float y) override;

    virtual std::pair<float, float> getPosition() const override;
    virtual void setPosition(float x, float y) override;

    virtual std::pair<float, float> getSize() const override;
    virtual void setSize(float width, float height) override;

    virtual bool isVisible() const override;
    virtual void setVisible(bool visible) override;

private:
    std::shared_ptr<IBannerAd> ad_;

    bool loading_;
    bool loaded_;
    std::unique_ptr<ObserverHandle> handle_;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif // EE_X_GUARDED_BANNER_AD_HPP