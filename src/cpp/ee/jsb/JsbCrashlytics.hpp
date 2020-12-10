//
//  jsb_crashlytics.hpp
//  ee-x
//
//  Created by Duc Nguyen on 7/17/18.
//

#ifndef EE_X_JSB_CRASHLYTICS_HPP
#define EE_X_JSB_CRASHLYTICS_HPP

#include <ee/jsb/JsbCoreFwd.hpp>

namespace ee {
namespace crashlytics {
bool registerJsb(se::Object* global);
} // namespace crashlytics
} // namespace ee

#endif // EE_X_JSB_CRASHLYTICS_HPP
