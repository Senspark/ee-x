//
//  NullAdView.hpp
//  ee_x
//
//  Created by Zinge on 10/27/17.
//
//

#ifndef EE_X_NULL_AD_VIEW_HPP
#define EE_X_NULL_AD_VIEW_HPP

#include "ee/ads/IAdView.hpp"

namespace ee {
namespace ads {
class NullAdView : public IAdView {
public:
    NullAdView();

    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual std::pair<float, float> getAnchor() const override;

    virtual void setAnchor(float x, float y) override;

    virtual std::pair<int, int> getPosition() const override;

    virtual void setPosition(int x, int y) override;

    virtual std::pair<int, int> getSize() const override;

    virtual void setSize(int width, int height) override;

    virtual void setVisible(bool visible) override;

private:
    int positionX_;
    int positionY_;
    float anchorX_;
    float anchorY_;
    int width_;
    int height_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_NULL_AD_VIEW_HPP */
