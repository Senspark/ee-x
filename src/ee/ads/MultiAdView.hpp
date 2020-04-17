//
//  MultiBannerAd.hpp
//  ee_x
//
//  Created by Zinge on 10/18/17.
//
//

#ifndef EE_X_MULTI_AD_VIEW_HPP
#define EE_X_MULTI_AD_VIEW_HPP

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
    std::shared_ptr<IAdView> activeItem_;

    /// Custom anchor.
    std::pair<float, float> anchor_;

    /// Custom position.
    std::pair<int, int> position_;

    /// Custom size.
    std::pair<int, int> size_;

    /// Whether to use custom size.
    bool useCustomSize_;

    /// Whether visible.
    bool visible_;

    /// Whether the active item is new.
    bool new_;

    std::vector<std::shared_ptr<IAdView>> items_;

    std::set<std::shared_ptr<IAdView>> loadedItems_;

    std::unique_ptr<ObserverHandle> handle_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_MULTI_AD_VIEW_HPP */
