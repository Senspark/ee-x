//
//  AdViewHelper.hpp
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#ifndef EE_X_AD_VIEW_HELPER_HPP
#define EE_X_AD_VIEW_HELPER_HPP

#include <string>

#include <ee/core/ObserverManager.hpp>

#include "ee/ads/IAdView.hpp"

namespace ee {
namespace ads {
class AdViewHelper : public IAdView, public ObserverManager<IAdViewObserver> {
public:
    explicit AdViewHelper(IMessageBridge& bridge,    //
                          const std::string& prefix, //
                          const std::string& adId);

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
    std::string k__isLoaded() const;
    std::string k__load() const;
    std::string k__getPosition() const;
    std::string k__setPosition() const;
    std::string k__getSize() const;
    std::string k__setSize() const;
    std::string k__isVisible() const;
    std::string k__setVisible() const;

    std::pair<int, int> getPositionTopLeft() const;

    void setPositionTopLeft(int x, int y);

    IMessageBridge& bridge_;
    std::string prefix_;
    std::string adId_;

    float anchorX_;
    float anchorY_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_AD_VIEW_HELPER_HPP */
