//
//  jsb_crashlytics.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/jsb_crashlytics.hpp"

#include "ee/jsb/crashlytics/jsb_crashlytics_bridge.hpp"

namespace ee {
namespace crashlytics {
bool register_crashlytics_manual(se::Object* object) {
    register_crashlytics_bridge_manual(object);
    return true;
}
} // namespace crashlytics
} // namespace ee
