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
    using SizeProvider = std::function<std::pair<int, int>()>;

    explicit AdViewHelper(IMessageBridge& bridge, const MessageHelper& helper,
                          const SizeProvider& sizeProvider);

    bool isLoaded() const;
    void load();

    std::pair<float, float> getAnchor() const;
    void setAnchor(float x, float y);

    std::pair<int, int> getPosition() const;
    void setPosition(int x, int y);

    std::pair<int, int> getSize() const;
    void setSize(int width, int height);

    bool isVisible() const;
    void setVisible(bool visible);

private:
    std::pair<int, int> getPositionTopLeft() const;
    void setPositionTopLeft(int x, int y);

    IMessageBridge& bridge_;
    MessageHelper helper_;
    SizeProvider sizeProvider_;

    float anchorX_;
    float anchorY_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_AD_VIEW_HELPER_HPP */
