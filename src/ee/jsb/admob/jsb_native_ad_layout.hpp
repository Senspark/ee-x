//
//  jsb_native_ad_layout.hpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/15/18.
//

#ifndef JSB_NATIVE_AD_LAYOUT_HPP__
#define JSB_NATIVE_AD_LAYOUT_HPP__

#include "ee/jsb/core/jsb_templates.hpp"
#include "ee/jsb/jsb_admob_fwd.hpp"
#include "ee/jsb/jsb_fwd.hpp"

namespace ee {
namespace core {
template <>
ee::admob::NativeAdLayout
get_value<ee::admob::NativeAdLayout>(const se::Value& value);
} // namespace core

namespace admob {
bool register_native_ad_layout_manual(se::Object* object);
} // namespace admob

} // namespace ee

#endif /* JSB_NATIVE_AD_LAYOUT_HPP__ */
