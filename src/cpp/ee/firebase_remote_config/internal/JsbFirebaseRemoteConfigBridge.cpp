//
// jsb_firebase_remote_config.cpp
// ee_x
//
//  Created by Nguyen Van Quynh on 9/23/18
//
//

#include "ee/firebase_remote_config/internal/JsbFirebaseRemoteConfigBridge.hpp"

#ifdef EE_X_COCOS_JS
#include <ee/core/JsbUtils.hpp>

#include "ee/firebase_remote_config/FirebaseRemoteConfigBridge.hpp"

namespace ee {
namespace firebase {
namespace remote_config {
namespace {
se::Class* clazz = nullptr;

using Self = Bridge;

// clang-format off
constexpr auto constructor      = &JsbUtils::makeConstructor<Self>;
constexpr auto finalize         = &JsbUtils::makeFinalize<Self>;
constexpr auto initialize       = &JsbUtils::makeMethod<&Self::initialize>;
constexpr auto activateFetched  = &JsbUtils::makeMethod<&Self::activateFetched>;
constexpr auto fetchOnly        = &JsbUtils::makeMethod<&Self::fetchOnly>;
constexpr auto fetch            = &JsbUtils::makeMethod<&Self::fetch>;
constexpr auto getInfo          = &JsbUtils::makeMethod<&Self::getInfo>;
constexpr auto getInfoJsb       = &JsbUtils::makeMethod<&Self::getInfoJsb>;
constexpr auto setDefaultBool   = &JsbUtils::makeMethod<&Self::setDefaultBool>;
constexpr auto setDefaultLong   = &JsbUtils::makeMethod<&Self::setDefaultLong>;
constexpr auto setDefaultDouble = &JsbUtils::makeMethod<&Self::setDefaultDouble>;
constexpr auto setDefaultString = &JsbUtils::makeMethod<&Self::setDefaultString>;
constexpr auto flushDefaults    = &JsbUtils::makeMethod<&Self::flushDefaults>;
constexpr auto getBool          = &JsbUtils::makeMethod<&Self::getBool>;
constexpr auto getLong          = &JsbUtils::makeMethod<&Self::getLong>;
constexpr auto getDouble        = &JsbUtils::makeMethod<&Self::getDouble>;
constexpr auto getString        = &JsbUtils::makeMethod<&Self::getString>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(initialize)
SE_BIND_FUNC(activateFetched)
SE_BIND_FUNC(fetchOnly)
SE_BIND_FUNC(fetch)
SE_BIND_FUNC(getInfo)
SE_BIND_FUNC(getInfoJsb)
SE_BIND_FUNC(setDefaultBool)
SE_BIND_FUNC(setDefaultLong)
SE_BIND_FUNC(setDefaultDouble)
SE_BIND_FUNC(setDefaultString)
SE_BIND_FUNC(flushDefaults)
SE_BIND_FUNC(getBool)
SE_BIND_FUNC(getLong)
SE_BIND_FUNC(getDouble)
SE_BIND_FUNC(getString)
} // namespace

bool registerJsbBridge(se::Object* global) {
    auto scope = JsbUtils::getPath(global, "ee");
    auto cls = se::Class::create("FirebaseRemoteConfig", scope, nullptr,
                                 _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, initialize);
    EE_JSB_DEFINE_FUNCTION(cls, activateFetched);
    EE_JSB_DEFINE_FUNCTION(cls, fetchOnly);
    EE_JSB_DEFINE_FUNCTION(cls, fetch);
    EE_JSB_DEFINE_FUNCTION(cls, getInfo);
    EE_JSB_DEFINE_FUNCTION(cls, getInfoJsb);
    EE_JSB_DEFINE_FUNCTION(cls, setDefaultBool);
    EE_JSB_DEFINE_FUNCTION(cls, setDefaultLong);
    EE_JSB_DEFINE_FUNCTION(cls, setDefaultDouble);
    EE_JSB_DEFINE_FUNCTION(cls, setDefaultString);
    EE_JSB_DEFINE_FUNCTION(cls, flushDefaults);
    EE_JSB_DEFINE_FUNCTION(cls, getBool);
    EE_JSB_DEFINE_FUNCTION(cls, getLong);
    EE_JSB_DEFINE_FUNCTION(cls, getDouble);
    EE_JSB_DEFINE_FUNCTION(cls, getString);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace remote_config
} // namespace firebase
} // namespace ee

#endif // EE_X_COCOS_JS