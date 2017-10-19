//
//  AdViewBridgeHelper.hpp
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#ifndef EE_X_AD_VIEW_BRIDGE_HELPER_HPP
#define EE_X_AD_VIEW_BRIDGE_HELPER_HPP

#include "ee/ads/AdViewInterface.hpp"
#include "ee/ads/internal/AdViewHelper.hpp"

namespace ee {
namespace ads {
class AdViewHelper;

class AdViewBridgeHelper : public AdViewInterface {
public:
    explicit AdViewBridgeHelper(const AdViewHelper& helper);

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
    std::pair<int, int> getPositionTopLeft() const;

    void setPositionTopLeft(int x, int y);

    float anchorX_;
    float anchorY_;
    AdViewHelper helper_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_AD_VIEW_BRIDGE_HELPER_HPP */
