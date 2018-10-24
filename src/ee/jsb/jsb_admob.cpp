//
//  jsb_admob.cpp
//  ee-x
//
//  Created by Le Van Kiet on 10/24/18.
//

#include "ee/jsb/admob/jsb_admob_bridge.hpp"
#include "ee/jsb/admob/jsb_native_ad_layout.hpp"
#include "ee/jsb/jsb_admob.hpp"

namespace ee {
namespace admob {
bool register_all_admob_manual(se::Object* object) {
    register_admob_bridge_manual(object);
    register_native_ad_layout_manual(object);
    return true;
}
} // namespace admob
} // namespace ee
