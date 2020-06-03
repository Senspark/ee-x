#ifndef EE_X_GUARDED_AD_VIEW_HPP
#define EE_X_GUARDED_AD_VIEW_HPP

#ifdef __cplusplus

#include <optional>

#include <ee/core/SafeObserverManager.hpp>

#include "ee/ads/IAdView.hpp"

namespace ee {
namespace ads {
class GuardedAdView : public IAdView,
                      public SafeObserverManager<IAdViewObserver> {
public:
    explicit GuardedAdView(const std::shared_ptr<IAdView>& ad);
    virtual ~GuardedAdView() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;

    virtual std::pair<float, float> getAnchor() const override;
    virtual void setAnchor(float x, float y) override;

    virtual std::pair<int, int> getPosition() const override;
    virtual void setPosition(int x, int y) override;

    virtual std::pair<int, int> getSize() const override;
    virtual void setSize(int width, int height) override;

    virtual bool isVisible() const override;
    virtual void setVisible(bool visible) override;

private:
    std::shared_ptr<IAdView> ad_;

    bool loading_;
    bool loaded_;
    bool visible_;
    mutable std::optional<std::pair<int, int>> size_;
    std::unique_ptr<ObserverHandle> handle_;
    std::unique_ptr<SpinLock> lock_;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif // EE_X_GUARDED_AD_VIEW_HPP
