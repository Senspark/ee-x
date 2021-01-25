//
//  JsbFirebaseFetchFailureReason.cpp
//  ee_x
//
//  Created by eps on 10/16/19
//

#include "ee/firebase_remote_config/internal/JsbFirebaseFetchFailureReason.hpp"

#ifdef EE_X_COCOS_JS
#include <ee/core/JsbUtils.hpp>

#include "ee/firebase_remote_config/FirebaseRemoteConfigBridge.hpp"

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
    auto scope = JsbUtils::getPath(global, "ee");
    auto cls = JsbUtils::getPath(scope, "FirebaseFetchFailureReason");
    cls->setProperty("Invalid",
                     se::Value(static_cast<UnderlyingType>(Self::Invalid)));
    cls->setProperty("Throttled",
                     se::Value(static_cast<UnderlyingType>(Self::Throttled)));
    cls->setProperty("Error",
                     se::Value(static_cast<UnderlyingType>(Self::Error)));

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace remote_config
} // namespace firebase
} // namespace ee

#endif // EE_X_COCOS_JS