//
//  jsb_recorder.cpp
//  ee-x
//
//  Created by HanPx on 01/12/19.
//

#include "ee/jsb/jsb_recorder.hpp"

#include "ee/jsb/recorder/jsb_recorder_bridge.hpp"

namespace ee {
namespace recorder {
bool register_all_recorder_manual(se::Object* object) {
    register_recorder_bridge_manual(object);
    return true;
}
} // namespace recorder
} // namespace ee
