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
                          const std::pair<int, int>& size);

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

    const std::pair<int, int> getSizeInternal() const;
    void setSizeInternal(int width, int height);

    const std::pair<float, float> getAnchorInternal() const;
    void setAnchorInternal(float x, float y);

    const std::pair<int, int> getPositionInternal() const;
    void setPositionInternal(int x, int y);

    bool isVisibleInternal() const;
    void setVisibleInternal(bool visible);

    IMessageBridge& bridge_;
    MessageHelper helper_;

    std::pair<float, float> anchor_;
    std::pair<int, int> position_; ///< @note Top left position.
    std::pair<int, int> size_;
    bool visible_;
    std::unique_ptr<SpinLock> lock_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_AD_VIEW_HELPER_HPP */
