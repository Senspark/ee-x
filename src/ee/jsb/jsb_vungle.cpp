//
//  jsb_vungle.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/jsb_vungle.hpp"

#include "ee/jsb/vungle/jsb_vungle_bridge.hpp"

namespace ee {
namespace vungle {
bool register_vungle_manual(se::Object* object) {
    register_vungle_bridge_manual(object);
    return true;
}
} // namespace vungle
} // namespace ee
