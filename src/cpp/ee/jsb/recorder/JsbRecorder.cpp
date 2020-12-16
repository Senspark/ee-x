//
//  jsb_recorder.cpp
//  ee-x
//
//  Created by HanPx on 01/12/19.
//

#include "ee/jsb/JsbRecorder.hpp"

#include "ee/jsb/recorder/JsbRecorderBridge.hpp"

namespace ee {
namespace recorder {
bool registerJsb(se::Object* global) {
    registerJsbBridge(global);
    return true;
}
} // namespace recorder
} // namespace ee
