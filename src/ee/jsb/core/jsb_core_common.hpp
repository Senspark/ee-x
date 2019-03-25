//
//  jsb_common.hpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/13/18.
//

#ifndef EE_X_JSB_COMMON_HPP
#define EE_X_JSB_COMMON_HPP

#include <string>

#include "ee/jsb/jsb_fwd.hpp"

namespace ee {
namespace core {
bool getOrCreatePlainObject_r(const char* name, se::Object* parent,
                              se::Object** outObj);

se::Object* getPath(se::Object* parent, const std::string& path);
} // namespace core
} // namespace ee

#endif /* EE_X_JSB_COMMON_HPP */
