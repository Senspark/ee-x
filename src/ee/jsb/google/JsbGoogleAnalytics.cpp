//
//  jsb_google_analytics.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/JsbGoogleAnalytics.hpp"

#include "ee/jsb/google/jsb_google_analytics_bridge.hpp"
#include "ee/jsb/google/jsb_google_analytics_event_builder.hpp"
#include "ee/jsb/google/jsb_google_analytics_tracker.hpp"

namespace ee {
namespace google {
bool registerJsb(se::Object* global) {
    register_google_bridge_manual(global);
    register_google_tracker_manual(global);
    register_google_event_builder_manual(global);
    return true;
}
} // namespace google
} // namespace ee
