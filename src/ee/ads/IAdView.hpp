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
#include <string>
#include <utility>

#include "ee/AdsFwd.hpp"

namespace ee {
namespace ads {
using AdViewCallback = std::function<void(bool result)>;
using OnClickedCallback = std::function<void()>;

class IAdView {
public:
    IAdView() = default;

    virtual ~IAdView() = default;

    /// Checks whether this ad view is loaded.
    virtual bool isLoaded() const = 0;

    virtual void load() = 0;

    virtual std::pair<float, float> getAnchor() const = 0;

    virtual void setAnchor(float x, float y) = 0;

    virtual std::pair<int, int> getPosition() const = 0;

    /// Moves this ad view to the specified position.
    /// @param[in] x The horizontal position in pixels.
    /// @param[in] y The vertical position in pixels.
    virtual void setPosition(int x, int y) = 0;

    /// Gets this ad view's size in pixels.
    /// @return A pair of width and height in pixels.
    virtual std::pair<int, int> getSize() const = 0;

    virtual void setSize(int width, int height) = 0;

    /// Sets this ad view's visibility.
    /// @param[in] visible Whether this ad view is visible.
    virtual void setVisible(bool visible) = 0;

    void setLoadCallback(const AdViewCallback& callback);
    virtual void setOnClickedCallback(const OnClickedCallback& callback);

protected:
    void setLoadResult(bool result);
    void performClick();

private:
    AdViewCallback callback_{nullptr};
    OnClickedCallback onClickCallback_{nullptr};
};
} // namespace ads
} // namespace ee

#endif /* EE_X_I_AD_VIEW_HPP */
