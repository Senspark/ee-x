//
//  jsb_crashlytics.hpp
//  ee-x
//
//  Created by Duc Nguyen on 7/17/18.
//

#ifndef JSB_CRASHLYTICS_HPP__
#define JSB_CRASHLYTICS_HPP__

#include "ee/jsb/jsb_fwd.hpp"

namespace ee {
namespace crashlytics {
bool register_crashlytics_manual(se::Object* object);
} // namespace crashlytics
} // namespace ee

#endif // JSB_CRASHLYTICS_HPP
