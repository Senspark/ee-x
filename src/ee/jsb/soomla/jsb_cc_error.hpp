//
//  jsb_cc_error.hpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/13/18.
//

#ifndef JSB_CC_ERROR_HPP
#define JSB_CC_ERROR_HPP

#include <stdio.h>

namespace se {
class Object;
} // namespace se

namespace soomla {

bool register_cc_error_manual(se::Object* object);

} // namespace soomla

#endif /* JSB_CC_ERROR_HPP */
