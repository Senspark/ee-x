//
//  jsb_firebase.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/JsbFirebase.hpp"

#include "ee/jsb/firebase/remote_config/JsbFirebaseRemoteConfigBridge.hpp"
#include "ee/jsb/firebase/remote_config/JsbFirebaseConfigInfo.hpp"
#include "ee/jsb/firebase/remote_config/JsbFirebaseFetchFailureReason.hpp"
#include "ee/jsb/firebase/remote_config/JsbFirebaseLastFetchStatus.hpp"

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
