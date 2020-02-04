//
//  JsbAppsFlyer.cpp
//  ee-x
//
//  Created by eps on 12/05/19.
//

#include "ee/jsb/JsbAppsFlyer.hpp"

#include "ee/jsb/apps_flyer/JsbAppsFlyerBridge.hpp"
#include "ee/jsb/apps_flyer/JsbIAppsFlyerBridge.hpp"

namespace ee {
namespace apps_flyer {
bool registerJsb(se::Object* global) {
    registerJsbIBridge(global);
    registerJsbBridge(global);
    return true;
}
} // namespace apps_flyer
} // namespace ee
