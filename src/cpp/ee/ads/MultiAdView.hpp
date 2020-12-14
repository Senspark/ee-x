//
//  MultiBannerAd.hpp
//  ee_x
//
//  Created by Zinge on 10/18/17.
//
//

#ifndef EE_X_MULTI_AD_VIEW_HPP
#define EE_X_MULTI_AD_VIEW_HPP

#ifdef __cplusplus

#include <memory>
#include <set>
#include <utility>
#include <vector>

#include <ee/core/ObserverManager.hpp>

#include "ee/ads/IAdView.hpp"

namespace ee {
namespace ads {
class MultiAdView : public IAdView, public ObserverManager<IAdViewObserver> {
private:
    using Self = MultiAdView;

public:
    MultiAdView();
    virtual ~MultiAdView() override;

    Self& addItem(const std::shared_ptr<IAdView>& item);

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
    /// Custom anchor.
    std::pair<float, float> anchor_;

    /// Custom position.
    std::pair<int, int> position_;

    /// Whether visible.
    bool visible_;

    std::vector<std::shared_ptr<IAdView>> items_;
    std::unique_ptr<ObserverHandle> handle_;

    std::shared_ptr<IAdView> activeItem_;
    std::set<std::shared_ptr<IAdView>> loadedItems_;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_MULTI_AD_VIEW_HPP */