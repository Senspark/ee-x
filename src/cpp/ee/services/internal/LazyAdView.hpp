//
//  LazyAdView.hpp
//  ee-x-d542b565
//
//  Created by eps on 1/7/21.
//

#ifndef EE_X_LAZY_AD_VIEW_HPP
#define EE_X_LAZY_AD_VIEW_HPP

#include <optional>

#include <ee/ads/IAdView.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/services/ServicesFwd.hpp"

namespace ee {
namespace services {
class LazyAdView : public IAdView, public ObserverManager<AdObserver> {
public:
    LazyAdView();
    virtual ~LazyAdView() override;

    void setAd(const std::shared_ptr<IAdView>& ad);

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

    bool visible_;
    std::pair<float, float> anchor_;
    std::pair<float, float> position_;
    std::optional<std::pair<float, float>> size_;
    std::unique_ptr<ObserverHandle> handle_;
};
} // namespace services
} // namespace ee

#endif /* EE_X_LAZY_AD_VIEW_HPP */
