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

#include "ee/AdsFwd.hpp"

namespace ee {
namespace ads {
class AdViewHelper {
public:
    explicit AdViewHelper(IMessageBridge& bridge,    //
                          const std::string& prefix, //
                          const std::string& adId);

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
