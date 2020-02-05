//
//  jsb_core.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/JsbCore.hpp"

#include "ee/jsb/core/JsbLogLevel.hpp"
#include "ee/jsb/core/JsbLogger.hpp"
#include "ee/jsb/core/JsbMetrics.hpp"
#include "ee/jsb/core/JsbUtils.hpp"

namespace ee {
namespace core {
bool registerJsb(se::Object* global) {
    registerJsbLogLevel(global);
    registerJsbLogger(global);
    registerJsbMetrics(global);
    registerJsbUtils(global);
    return true;
}
} // namespace core
} // namespace ee
