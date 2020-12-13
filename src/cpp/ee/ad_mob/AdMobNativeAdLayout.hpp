//
//  AdMobNativeAdBuilder.hpp
//  ee_x
//
//  Created by Zinge on 10/16/17.
//
//

#ifndef EE_X_ADMOB_NATIVE_AD_LAYOUT_HPP
#define EE_X_ADMOB_NATIVE_AD_LAYOUT_HPP

#ifdef __cplusplus

#include <string>
#include <unordered_map>

#include "ee/ad_mob/AdMobFwd.hpp"

namespace ee {
namespace admob {
class NativeAdLayout {
private:
    using Self = NativeAdLayout;

public:
    NativeAdLayout();
    ~NativeAdLayout();

    Self& setBody(const std::string& id);
    Self& setCallToAction(const std::string& id);
    Self& setHeadline(const std::string& id);
    Self& setIcon(const std::string& id);
    Self& setImage(const std::string& id);
    Self& setMedia(const std::string& id);
    Self& setPrice(const std::string& id);
    Self& setStarRating(const std::string& id);
    Self& setStore(const std::string& id);

protected:
    friend Bridge;

    std::unordered_map<std::string, std::string> params_;
};
} // namespace admob
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_ADMOB_NATIVE_AD_LAYOUT_HPP */
