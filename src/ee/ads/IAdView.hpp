//
//  IAdView.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#ifndef EE_X_I_AD_VIEW_HPP
#define EE_X_I_AD_VIEW_HPP

#include <functional>
#include <utility>

#include <ee/CoroutineFwd.hpp>
#include <ee/core/IObserverManager.hpp>

#include "ee/AdsFwd.hpp"

namespace ee {
namespace ads {
struct IAdViewObserver {
    /// Occurs when this ad is loaded.
    std::function<void()> onLoaded;

    /// Occurs when the user clicks this ad.
    std::function<void()> onClicked;
};

class IAdView : public virtual IObserverManager<IAdViewObserver> {
public:
    virtual ~IAdView() = default;

    /// Destroys this ad.
    virtual void destroy() = 0;

    /// Checks whether this ad view is loaded.
    virtual bool isLoaded() const = 0;

    /// Attempts to load this ad view.
    virtual Task<bool> load() = 0;

    /// Gets the anchor of this ad view.
    virtual std::pair<float, float> getAnchor() const = 0;

    /// Sets the anchor of this ad view.
    /// @param x The horizontal anchor.
    /// @param y The vertical anchor.
    virtual void setAnchor(float x, float y) = 0;

    /// Gets the position in pixels.
    virtual std::pair<int, int> getPosition() const = 0;

    /// Moves this ad view to the specified position.
    /// @param[in] x The horizontal position in pixels.
    /// @param[in] y The vertical position in pixels.
    virtual void setPosition(int x, int y) = 0;

    /// Gets this ad view's size in pixels.
    /// @return A pair of width and height in pixels.
    virtual std::pair<int, int> getSize() const = 0;

    /// Sets the size in pixels.
    virtual void setSize(int width, int height) = 0;

    /// Checks whether this ad is visible.
    virtual bool isVisible() const = 0;

    /// Sets this ad view's visibility.
    /// @param[in] visible Whether this ad view is visible.
    virtual void setVisible(bool visible) = 0;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_I_AD_VIEW_HPP */
