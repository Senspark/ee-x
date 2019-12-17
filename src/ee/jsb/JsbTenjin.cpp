//
//  Created by eps on 12/6/19.
//

#include "ee/jsb/JsbTenjin.hpp"

#include "ee/jsb/tenjin/JsbTenjinBridge.hpp"
#include "ee/jsb/tenjin/JsbITenjinBridge.hpp"

namespace ee {
namespace tenjin {
bool registerJsb(se::Object* global) {
    registerJsbIBridge(global);
    registerJsbBridge(global);
    return true;
}
} // namespace tenjin
} // namespace ee
