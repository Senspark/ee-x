//
//  JsbFirebaseLastFetchStatus.cpp
//  ee_x
//
//  Created by eps on 10/16/19
//

#include "ee/jsb/firebase/JsbFirebaseLastFetchStatus.hpp"

#include "ee/firebase/FirebaseRemoteConfig.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

using Self = ee::firebase::LastFetchStatus;
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
bool registerLastFetchStatus(se::Object* global) {
    auto firebase_ = core::getPath(global, "ee/firebase");
    
    auto cls = core::getPath(firebase_, "LastFetchStatus");
    cls->setProperty("Success", se::Value(static_cast<UnderlyingType>(Self::Success)));
    cls->setProperty("Failure", se::Value(static_cast<UnderlyingType>(Self::Failure)));
    cls->setProperty("Pending", se::Value(static_cast<UnderlyingType>(Self::Pending)));

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace firebase
} // namespace ee