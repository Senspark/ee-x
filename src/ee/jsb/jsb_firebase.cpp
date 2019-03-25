//
//  jsb_firebase.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/jsb_firebase.hpp"

#include "ee/jsb/firebase/JsbFirebasePerformance.hpp"
#include "ee/jsb/firebase/JsbFirebasePerformanceTrace.hpp"
#include "ee/jsb/firebase/jsb_firebase_analytics.hpp"
#include "ee/jsb/firebase/jsb_firebase_remote_config.hpp"

namespace ee {
namespace firebase {
bool register_all_firebase_manual(se::Object* object) {
    register_firebase_analytics_manual(object);
    register_firebase_remote_config_manual(object);
    registerPerformance(object);
    registerPerformanceTrace(object);
    return true;
}
} // namespace firebase
} // namespace ee
