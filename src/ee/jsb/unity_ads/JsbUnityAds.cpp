//
//  jsb_unity_ads.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/JsbUnityAds.hpp"

#include "ee/jsb/unity_ads/JsbUnityAdsBridge.hpp"

namespace ee {
namespace unity_ads {
bool registerJsb(se::Object* global) {
    registerJsbBridge(global);
    return true;
}
} // namespace unity_ads
} // namespace ee
