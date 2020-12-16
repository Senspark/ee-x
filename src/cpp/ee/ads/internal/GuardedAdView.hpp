#ifndef EE_X_GUARDED_AD_VIEW_HPP
#define EE_X_GUARDED_AD_VIEW_HPP

#ifdef __cplusplus

#include <ee/core/ObserverManager.hpp>

#include "ee/ads/IAdView.hpp"

namespace ee {
namespace ads {
class GuardedAdView : public IAdView, public ObserverManager<IAdViewObserver> {
public:
    explicit GuardedAdView(const std::shared_ptr<IAdView>& ad);

    virtual ~GuardedAdView() override;

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
    std::shared_ptr<IAdView> ad_;

    bool loading_;
    bool loaded_;
    std::unique_ptr<ObserverHandle> handle_;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif // EE_X_GUARDED_AD_VIEW_HPP
