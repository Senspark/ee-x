//
//  jsb_adview.hpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/15/18.
//

#ifndef JSB_ADVIEW_HPP__
#define JSB_ADVIEW_HPP__

#include "ee/jsb/core/jsb_templates.hpp"

#include <ee/AdsFwd.hpp>

namespace se {
class Object;
class Value;
} // namespace se

namespace ee {
namespace core {
template <>
void set_value(se::Value& value, std::shared_ptr<ee::IAdView> input);
} // namespace core

namespace ads {

extern se::Object* __adsObj;

bool register_adview_manual(se::Object* object);
} // namespace ads
} // namespace ee

#endif /* JSB_ADVIEW_HPP__ */
