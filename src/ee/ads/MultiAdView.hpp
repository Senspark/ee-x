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
#include <vector>

#include "ee/ads/AdViewInterface.hpp"

namespace ee {
namespace ads {
class MultiAdView : public AdViewInterface {
private:
    using Self = MultiAdView;

public:
    MultiAdView();

    virtual ~MultiAdView() override;

    Self& addItem(const std::shared_ptr<AdViewInterface>& item);

    /// @see Super.
    virtual bool isLoaded() const override;

    /// @see Super.
    virtual void load() override;

    /// @see Super.
    virtual std::pair<float, float> getAnchor() const override;

    /// @see Super.
    virtual void setAnchor(float x, float y) override;

    /// @see Super.
    virtual std::pair<int, int> getPosition() const override;

    /// @see Super.
    virtual void setPosition(int x, int y) override;

    /// @see Super.
    virtual std::pair<int, int> getSize() const override;

    /// @see Super.
    virtual void setSize(int width, int height) override;

    /// @see Super.
    virtual void setVisible(bool visible) override;

private:
    void findActiveItem() const;

    mutable std::shared_ptr<AdViewInterface> activeItem_;

    std::pair<float, float> anchor_;
    std::pair<int, int> position_;
    std::pair<int, int> size_;
    bool visible_;
    std::vector<std::shared_ptr<AdViewInterface>> items_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_MULTI_AD_VIEW_HPP */
