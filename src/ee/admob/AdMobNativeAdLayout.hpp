//
//  AdMobNativeAdBuilder.hpp
//  ee_x
//
//  Created by Zinge on 10/16/17.
//
//

#ifndef EE_X_ADMOB_NATIVE_AD_BUILDER_HPP
#define EE_X_ADMOB_NATIVE_AD_BUILDER_HPP

#include <string>
#include <unordered_map>

#include "ee/AdMobFwd.hpp"

namespace ee {
namespace admob {
class NativeAdLayout {
private:
    using Self = NativeAdLayout;

public:
    NativeAdLayout();
    ~NativeAdLayout();

    Self& setLayoutName(const std::string& layoutName);

protected:
    friend AdMob;

    std::unordered_map<std::string, std::string> params_;
};
} // namespace admob
} // namespace ee

#endif /* AdMobNativeAdBuilder_hpp */
