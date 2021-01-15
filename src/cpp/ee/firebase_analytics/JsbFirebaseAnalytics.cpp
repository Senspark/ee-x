//
//  jsb_firebase.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/firebase_analytics/JsbFirebaseAnalytics.hpp"

#include "ee/firebase_analytics/internal/JsbFirebaseAnalyticsBridge.hpp"

namespace ee {
namespace firebase {
namespace analytics {
bool registerJsb(se::Object* global) {
    registerJsbBridge(global);
    return true;
}
} // namespace analytics
} // namespace firebase
} // namespace ee
