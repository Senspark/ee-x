//
//  jsb_firebase.hpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 8/22/18
//

#ifndef JSB_FIREBASE_HPP__
#define JSB_FIREBASE_HPP__

#ifdef __cplusplus
#include "ee/jsb/firebase/jsb_firebase_analytics.hpp"
#include "ee/jsb/firebase/jsb_firebase_remote_config.hpp"

namespace ee {
namespace firebase {
bool register_all_firebase_manual(se::Object* object) {
    register_firebase_analytics_manual(object);
    register_firebase_remote_config_manual(object);
    return true;
} // namespace register_all_firebase_manual
} // namespace firebase
} // namespace ee

#endif // __cplusplus
#endif // JSB_FIREBASE_HPP__
