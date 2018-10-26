//
//  jsb_google_analytics.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/jsb_google_analytics.hpp"

#include "ee/jsb/google/jsb_google_analytics_bridge.hpp"
#include "ee/jsb/google/jsb_google_analytics_event_builder.hpp"
#include "ee/jsb/google/jsb_google_analytics_tracker.hpp"

namespace ee {
namespace google {
bool register_all_google_analytics_manual(se::Object* object) {
    register_google_bridge_manual(object);
    register_google_tracker_manual(object);
    register_google_event_builder_manual(object);
    return true;
}
} // namespace google
} // namespace ee
