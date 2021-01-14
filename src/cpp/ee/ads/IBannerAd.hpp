//
//  IAdView.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#ifndef EE_X_I_BANNER_AD_HPP
#define EE_X_I_BANNER_AD_HPP

#ifdef __cplusplus

#include <utility>

#include "ee/ads/IAd.hpp"

namespace ee {
namespace ads {
class IBannerAd : public IAd {
public:
    virtual ~IBannerAd() = default;

    /// Gets the anchor of this ad view.
    virtual std::pair<float, float> getAnchor() const = 0;

    /// Sets the anchor of this ad view.
    /// @param x The horizontal anchor.
    /// @param y The vertical anchor.
    virtual void setAnchor(float x, float y) = 0;

    /// Gets the position in pixels.
    virtual std::pair<float, float> getPosition() const = 0;

    /// Moves this ad view to the specified position.
    /// @param[in] x The horizontal position in pixels.
    /// @param[in] y The vertical position in pixels.
    virtual void setPosition(float x, float y) = 0;

    /// Gets this ad view's size in pixels.
    /// @return A pair of width and height in pixels.
    virtual std::pair<float, float> getSize() const = 0;

    /// Sets the size in pixels.
    virtual void setSize(float width, float height) = 0;

    /// Checks whether this ad is visible.
    virtual bool isVisible() const = 0;

    /// Sets this ad view's visibility.
    /// @param[in] visible Whether this ad view is visible.
    virtual void setVisible(bool visible) = 0;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_I_BANNER_AD_HPP */
