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

namespace ee {
namespace ads {
class AdViewHelper {
public:
    explicit AdViewHelper(const std::string& prefix, const std::string& adId);

    std::string k__isLoaded() const;
    std::string k__load() const;
    std::string k__getPosition() const;
    std::string k__setPosition() const;
    std::string k__getSize() const;
    std::string k__setSize() const;
    std::string k__isVisible() const;
    std::string k__setVisible() const;

private:
    std::string prefix_;
    std::string adId_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_AD_VIEW_HELPER_HPP */
