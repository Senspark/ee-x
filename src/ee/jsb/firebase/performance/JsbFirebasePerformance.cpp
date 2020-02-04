//
//  jsb_firebase.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/JsbFirebase.hpp"

#include "ee/jsb/firebase/performance/JsbFirebasePerformanceBridge.hpp"
#include "ee/jsb/firebase/performance/JsbFirebasePerformanceTrace.hpp"

namespace ee {
namespace firebase {
namespace performance {
bool registerJsb(se::Object* global) {
    registerJsbBridge(global);
    registerJsbTrace(global);
    return true;
}
} // namespace performance
} // namespace firebase
} // namespace ee
