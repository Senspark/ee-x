//
//  JsbAppsFlyer.cpp
//  ee-x
//
//  Created by eps on 12/05/19.
//

#include "ee/jsb/JsbAppsFlyer.hpp"

#include "ee/jsb/appsflyer/JsbAppsFlyerBridge.hpp"
#include "ee/jsb/appsflyer/JsbIAppsFlyerBridge.hpp"

namespace ee {
namespace appsflyer {
bool registerJsb(se::Object* global) {
    registerJsbIBridge(global);
    registerJsbBridge(global);
    return true;
}
} // namespace appsflyer
} // namespace ee
