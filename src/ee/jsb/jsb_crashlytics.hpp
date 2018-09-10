//
//  jsb_crashlytics.hpp
//  ee-x
//
//  Created by Duc Nguyen on 7/17/18.
//

#ifndef JSB_CRASHLYTICS_HPP__
#define JSB_CRASHLYTICS_HPP__

#ifdef __cplusplus
#include "ee/jsb/crashlytics/jsb_crashlytics_bridge.hpp"

namespace ee {
namespace crashlytics {
bool register_crashlytics_manual(se::Object* object) {
    register_crashlytics_bridge_manual(object);
    return true;
}
} // namespace register_crashlytics_manual
} // namespace ee

#endif // __cplusplus

#endif // JSB_CRASHLYTICS_HPP
