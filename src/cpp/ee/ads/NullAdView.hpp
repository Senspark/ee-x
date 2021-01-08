//
//  NullAdView.hpp
//  ee_x
//
//  Created by Zinge on 10/27/17.
//
//

#ifndef EE_X_NULL_AD_VIEW_HPP
#define EE_X_NULL_AD_VIEW_HPP

#ifdef __cplusplus

#include <ee/core/ObserverManager.hpp>

#include "ee/ads/IAdView.hpp"

namespace ee {
namespace ads {
class NullAdView : public IAdView, public ObserverManager<AdObserver> {
public:
    NullAdView();
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
    bool loaded_;
    int positionX_;
    int positionY_;
    float anchorX_;
    float anchorY_;
    int width_;
    int height_;
    bool visible_;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_NULL_AD_VIEW_HPP */
