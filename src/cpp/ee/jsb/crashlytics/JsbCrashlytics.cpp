//
//  jsb_crashlytics.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/JsbCrashlytics.hpp"

#include "ee/jsb/crashlytics/JsbCrashlyticsBridge.hpp"

namespace ee {
namespace crashlytics {
bool registerJsb(se::Object* global) {
    registerJsbBridge(global);
    return true;
}
} // namespace crashlytics
} // namespace ee
