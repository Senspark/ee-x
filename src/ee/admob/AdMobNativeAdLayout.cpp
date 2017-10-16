//
//  AdMobNativeAdBuilder.cpp
//  ee_x
//
//  Created by Zinge on 10/16/17.
//
//

#include "ee/admob/AdMobNativeAdLayout.hpp"

namespace ee {
namespace admob {
namespace {
constexpr auto k__layout_name = "layout_name";
} // namespace

#define Self NativeAdLayout

Self::NativeAdLayout() {}
Self::~NativeAdLayout() {}

Self& Self::setLayoutName(const std::string& layoutName) {
    params_[k__layout_name] = layoutName;
    return *this;
}

#undef Self
} // namespace admob
} // namespace ee
