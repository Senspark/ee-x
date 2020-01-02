//
//  jsb_recorder_bridge.cpp
//  ee_x
//
//  Created by HanPx on 01/12/19.
//
#include "ee/jsb/recorder/jsb_recorder_bridge.hpp"

#include "ee/Recorder.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace recorder {
namespace {
se::Class* clazz = nullptr;

using Self = Recorder;

// clang-format off
constexpr auto constructor              = &core::makeConstructor<Self>;
constexpr auto finalize                 = &core::makeFinalize<Self>;
constexpr auto startRecording           = &core::makeMethod<&Self::startRecording>;
constexpr auto stopRecording            = &core::makeMethod<&Self::stopRecording>;
constexpr auto cancelRecording          = &core::makeMethod<&Self::cancelRecording>;
constexpr auto getRecordingUrl          = &core::makeMethod<&Self::getRecordingUrl>;
constexpr auto checkRecordingPermission = &core::makeMethod<&Self::checkRecordingPermission>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(startRecording)
SE_BIND_FUNC(stopRecording)
SE_BIND_FUNC(cancelRecording)
SE_BIND_FUNC(getRecordingUrl)
SE_BIND_FUNC(checkRecordingPermission)
} // namespace

bool register_recorder_bridge_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* recorderObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("recorder", eeObj, &recorderObj);

    auto cls =
        se::Class::create("Recorder", recorderObj, nullptr, _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, startRecording);
    EE_JSB_DEFINE_FUNCTION(cls, stopRecording);
    EE_JSB_DEFINE_FUNCTION(cls, cancelRecording);
    EE_JSB_DEFINE_FUNCTION(cls, getRecordingUrl);
    EE_JSB_DEFINE_FUNCTION(cls, checkRecordingPermission);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace recorder
} // namespace ee
