//
//  jsb_unity_ads.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/jsb_unity_ads.hpp"

#include "ee/jsb/unityads/jsb_unity_ads_bridge.hpp"

namespace ee {
namespace unityads {
bool register_unity_ads_manual(se::Object* object) {
    register_unity_ads_bridge_manual(object);
    return true;
}
} // namespace unityads
} // namespace ee
