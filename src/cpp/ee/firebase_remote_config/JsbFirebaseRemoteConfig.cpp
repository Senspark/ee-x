//
//  jsb_firebase.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/firebase_remote_config/JsbFirebaseRemoteConfig.hpp"

#include "ee/firebase_remote_config/internal/JsbFirebaseConfigInfo.hpp"
#include "ee/firebase_remote_config/internal/JsbFirebaseFetchFailureReason.hpp"
#include "ee/firebase_remote_config/internal/JsbFirebaseLastFetchStatus.hpp"
#include "ee/firebase_remote_config/internal/JsbFirebaseRemoteConfigBridge.hpp"

namespace ee {
namespace firebase {
namespace remote_config {
bool registerJsb(se::Object* global) {
    registerJsbBridge(global);
    registerJsbConfigInfo(global);
    registerJsbFetchFailureReason(global);
    registerJsbLastFetchStatus(global);
    return true;
}
} // namespace remote_config
} // namespace firebase
} // namespace ee
