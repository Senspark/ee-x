//
// jsb_firebase_remote_config.cpp
// ee_x
//
//  Created by Nguyen Van Quynh on 9/23/18
//
//

#include "ee/jsb/firebase/jsb_firebase_remote_config.hpp"

#include "ee/Firebase.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace firebase {
namespace {
se::Class* clazz = nullptr;

using Self = RemoteConfig;

// clang-format off
constexpr auto constructor      = &core::makeConstructor<Self>;
constexpr auto finalize         = &core::makeFinalize<Self>;
constexpr auto initialize       = &core::makeMethod<&Self::initialize>;
constexpr auto activateFetched  = &core::makeMethod<&Self::activateFetched>;
constexpr auto fetchOnly        = &core::makeMethod<&Self::fetchOnly>;
constexpr auto fetch            = &core::makeMethod<&Self::fetch>;
constexpr auto getInfo          = &core::makeMethod<&Self::getInfo>;
constexpr auto getInfoJsb       = &core::makeMethod<&Self::getInfoJsb>;
constexpr auto setDefaultBool   = &core::makeMethod<&Self::setDefaultBool>;
constexpr auto setDefaultLong   = &core::makeMethod<&Self::setDefaultLong>;
constexpr auto setDefaultDouble = &core::makeMethod<&Self::setDefaultDouble>;
constexpr auto setDefaultString = &core::makeMethod<&Self::setDefaultString>;
constexpr auto flushDefaults    = &core::makeMethod<&Self::flushDefaults>;
constexpr auto getBool          = &core::makeMethod<&Self::getBool>;
constexpr auto getLong          = &core::makeMethod<&Self::getLong>;
constexpr auto getDouble        = &core::makeMethod<&Self::getDouble>;
constexpr auto getString        = &core::makeMethod<&Self::getString>;
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

bool register_firebase_remote_config_manual(se::Object* global) {
    auto firebase_ = core::getPath(global, "ee/firebase");
    auto cls =
        se::Class::create("RemoteConfig", firebase_, nullptr, _SE(constructor));
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
} // namespace firebase
} // namespace ee
