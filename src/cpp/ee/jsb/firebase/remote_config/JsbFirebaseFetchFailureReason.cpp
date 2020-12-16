//
//  JsbFirebaseFetchFailureReason.cpp
//  ee_x
//
//  Created by eps on 10/16/19
//

#include "ee/jsb/firebase/remote_config/JsbFirebaseFetchFailureReason.hpp"

#include <ee/firebase/remote_config/FirebaseRemoteConfigBridge.hpp>
#include <ee/jsb/JsbCore.hpp>

using Self = ee::firebase::remote_config::FetchFailureReason;
using UnderlyingType = std::underlying_type_t<Self>;

namespace ee {
/*
Not used since ConfigInfo returns JSON object.

namespace core {
template <>
void set_value(se::Value& value, Self& input) {
    set_value(value, static_cast<UnderlyingType>(input));
}
} // namespace core
*/

namespace firebase {
namespace remote_config {
bool registerJsbFetchFailureReason(se::Object* global) {
    auto scope = core::getPath(global, "ee");
    auto cls = core::getPath(scope, "FirebaseFetchFailureReason");
    cls->setProperty("Invalid", se::Value(static_cast<UnderlyingType>(Self::Invalid)));
    cls->setProperty("Throttled", se::Value(static_cast<UnderlyingType>(Self::Throttled)));
    cls->setProperty("Error", se::Value(static_cast<UnderlyingType>(Self::Error)));

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace remote_config
} // namespace firebase
} // namespace ee