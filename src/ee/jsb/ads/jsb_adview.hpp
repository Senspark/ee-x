//
//  jsb_adview.hpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/15/18.
//

#ifndef JSB_ADVIEW_HPP__
#define JSB_ADVIEW_HPP__

#include <ee/AdsFwd.hpp>

#include "ee/jsb/core/jsb_templates.hpp"

namespace se {
class Object;
class Value;
} // namespace se

namespace ee {
namespace ads {

extern se::Object* __adsObj;

bool register_adview_manual(se::Object* object);
} // namespace ads
} // namespace ee

#endif /* JSB_ADVIEW_HPP__ */
