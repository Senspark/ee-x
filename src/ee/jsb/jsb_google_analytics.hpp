//
//  jsb_crashlytics.hpp
//  ee-x
//
//  Created by Duc Nguyen on 7/17/18.
//

#ifndef JSB_GOOGLE_ANALYTICS_HPP__
#define JSB_GOOGLE_ANALYTICS_HPP__

#ifdef __cplusplus
#include "ee/jsb/google/jsb_google_analytics_bridge.hpp"
#include "ee/jsb/google/jsb_google_analytics_tracker.hpp"

namespace ee {
namespace google {

bool register_all_google_analytics_manual(se::Object* object) {
    register_google_bridge_manual(object);
    register_google_tracker_manual(object);
    return true;
}

}
}

#endif // __cplusplus
#endif // JSB_GOOGLE_ANALYTICS_HPP__
