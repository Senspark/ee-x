//
//  jsb_core.hpp
//  ee-x
//
//  Created by Duc Nguyen on 7/16/18.
//

#ifndef EE_X_JSB_CORE_HPP
#define EE_X_JSB_CORE_HPP

#include "ee/jsb/JsbCoreFwd.hpp"
#include "ee/jsb/core/SharedPtrHandler.hpp"
#include "ee/jsb/core/Templates.hpp"
#include "ee/jsb/core/Utils.hpp"

namespace ee {
namespace core {
bool registerJsb(se::Object* global);
} // namespace core
} // namespace ee

#endif // EE_X_JSB_CORE_HPP
