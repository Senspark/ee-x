//
//  jsb_banner_ad_size.hpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/15/18.
//

#ifndef JSB_BANNER_AD_SIZE_HPP__
#define JSB_BANNER_AD_SIZE_HPP__

#include "ee/jsb/core/jsb_templates.hpp"

namespace se {
class Object;
class Value;
} // namespace se

namespace ee {
namespace admob {
enum class BannerAdSize;
} // namespace admob
} // namespace ee

namespace ee {
namespace core {
template <>
ee::admob::BannerAdSize
get_value<ee::admob::BannerAdSize>(const se::Value& value);

template <>
void set_value<ee::admob::BannerAdSize>(se::Value& value,
                                        ee::admob::BannerAdSize input);
} // namespace core

namespace admob {
bool register_banner_ad_size_manual(se::Object* object);
} // namespace admob

} // namespace ee

#endif /* JSB_BANNER_AD_SIZE_HPP__ */
