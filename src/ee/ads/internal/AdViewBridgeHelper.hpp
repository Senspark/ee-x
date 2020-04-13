//
//  AdViewBridgeHelper.hpp
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#ifndef EE_X_AD_VIEW_BRIDGE_HELPER_HPP
#define EE_X_AD_VIEW_BRIDGE_HELPER_HPP

#include <ee/core/ObserverManager.hpp>

#include "ee/AdsFwd.hpp"
#include "ee/ads/IAdView.hpp"
#include "ee/ads/internal/AdViewHelper.hpp"

namespace ee {
namespace ads {
class AdViewBridgeHelper : public IAdView,
                           public ObserverManager<IAdViewObserver> {
public:
    explicit AdViewBridgeHelper(IMessageBridge& bridge,
                                const AdViewHelper& helper);

    virtual bool isLoaded() const override;
    virtual void load() override;

    virtual std::pair<float, float> getAnchor() const override;
    virtual void setAnchor(float x, float y) override;

    virtual std::pair<int, int> getPosition() const override;
    virtual void setPosition(int x, int y) override;

    virtual std::pair<int, int> getSize() const override;
    virtual void setSize(int width, int height) override;

    virtual bool isVisible() const override;
    virtual void setVisible(bool visible) override;

private:
    std::pair<int, int> getPositionTopLeft() const;

    void setPositionTopLeft(int x, int y);

    float anchorX_;
    float anchorY_;
    IMessageBridge& bridge_;
    AdViewHelper helper_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_AD_VIEW_BRIDGE_HELPER_HPP */
