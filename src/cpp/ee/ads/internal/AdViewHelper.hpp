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
#include <utility>

#include "ee/ads/internal/MessageHelper.hpp"

namespace ee {
namespace ads {
class AdViewHelper {
public:
    explicit AdViewHelper(IMessageBridge& bridge, const MessageHelper& helper,
                          const std::pair<float, float>& size);

    bool isLoaded() const;
    void load();

    std::pair<float, float> getAnchor() const;
    void setAnchor(float x, float y);

    std::pair<float, float> getPosition() const;
    void setPosition(float x, float y);

    std::pair<float, float> getSize() const;
    void setSize(float width, float height);

    bool isVisible() const;
    void setVisible(bool visible);

private:
    std::pair<float, float> getPositionTopLeft() const;
    void setPositionTopLeft(float x, float y);

    const std::pair<float, float> getSizeInternal() const;
    void setSizeInternal(float width, float height);

    const std::pair<float, float> getAnchorInternal() const;
    void setAnchorInternal(float x, float y);

    const std::pair<float, float> getPositionInternal() const;
    void setPositionInternal(float x, float y);

    bool isVisibleInternal() const;
    void setVisibleInternal(bool visible);

    IMessageBridge& bridge_;
    MessageHelper helper_;

    std::pair<float, float> anchor_;
    std::pair<float, float> position_; ///< @note Top left position.
    std::pair<float, float> size_;
    bool visible_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_AD_VIEW_HELPER_HPP */
