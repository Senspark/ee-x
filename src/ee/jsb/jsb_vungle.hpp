//
// jsb_vungle.cpp
// ee_x
//
//  Created by Nguyen Van Quynh on 9/4/18
//
//

#ifndef JSB_VUNGLE_HPP__
#define JSB_VUNGLE_HPP__

#ifdef __cplusplus
#include "ee/jsb/vungle/jsb_vungle_bridge.hpp"

namespace ee {
namespace vungle {
bool register_vungle_manual(se::Object* object) {
    register_vungle_bridge_manual(object);
    return true;
} // namespace register_vungle_manual
} // namespace vungle
} // namespace ee

#endif // __cplusplus
#endif // JSB_VUNGLE_HPP__
