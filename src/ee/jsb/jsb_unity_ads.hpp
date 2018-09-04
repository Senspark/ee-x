//
// jsb_unity_ads.cpp
// ee_x
//
//  Created by Nguyen Van Quynh on 8/23/18
//
//

#ifndef JSB_UNITY_ADS_HPP__
#define JSB_UNITY_ADS_HPP__

#ifdef __cplusplus
#include "ee/jsb/unityads/jsb_unity_ads_bridge.hpp"

namespace ee {
namespace unityads {
bool register_unity_ads_manual(se::Object* object) {
    register_unity_ads_bridge_manual(object);
    return true;
} // namespace register_unity_ads_manual
} // namespace unityads
} // namespace ee

#endif // __cplusplus
#endif // JSB_UNITY_ADS_HPP__
