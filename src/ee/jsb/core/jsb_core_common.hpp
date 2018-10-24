//
//  jsb_common.hpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/13/18.
//

#ifndef JSB_COMMON_HPP__
#define JSB_COMMON_HPP__

#include "ee/jsb/jsb_fwd.hpp"

namespace ee {
namespace core {

extern se::Object* __coreObj;
extern se::Object* __eeObj;

bool getOrCreatePlainObject_r(const char* name, se::Object* parent,
                              se::Object** outObj);

} // namespace core
} // namespace ee

#endif /* jsb_common_hpp */
