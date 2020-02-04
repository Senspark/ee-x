//
//  jsb_admob.cpp
//  ee-x
//
//  Created by Le Van Kiet on 10/24/18.
//

#include "ee/jsb/JsbAdMob.hpp"

#include "ee/jsb/admob/JsbAdMobBridge.hpp"
#include "ee/jsb/admob/JsbAdMobNativeAdLayout.hpp"

namespace ee {
namespace admob {
bool registerJsb(se::Object* global) {
    registerJsbBridge(global);
    registerJsbNativeAdLayout(global);
    return true;
}
} // namespace admob
} // namespace ee
