//
//  jsb_vungle.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/JsbVungle.hpp"

#include "ee/jsb/vungle/JsbVungleBridge.hpp"

namespace ee {
namespace vungle {
bool registerJsb(se::Object* global) {
    registerJsbBridge(global);
    return true;
}
} // namespace vungle
} // namespace ee
