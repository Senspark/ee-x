//
//  JsbFirebaseFetchFailureReason.cpp
//  ee_x
//
//  Created by eps on 10/16/19
//

#include "ee/jsb/firebase/JsbFirebaseFetchFailureReason.hpp"

#include "ee/firebase/remoteconfig/FirebaseRemoteConfig.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

using Self = ee::firebase::FetchFailureReason;
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
bool registerFetchFailureReason(se::Object* global) {
    auto firebase_ = core::getPath(global, "ee/firebase");

    auto cls = core::getPath(firebase_, "FetchFailureReason");
    cls->setProperty("Invalid", se::Value(static_cast<UnderlyingType>(Self::Invalid)));
    cls->setProperty("Throttled", se::Value(static_cast<UnderlyingType>(Self::Throttled)));
    cls->setProperty("Error", se::Value(static_cast<UnderlyingType>(Self::Error)));

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace firebase
} // namespace ee