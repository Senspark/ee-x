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
constexpr auto startScreenRecording     = &core::makeMethod<&Self::startScreenRecording>;
constexpr auto stopScreenRecording      = &core::makeMethod<&Self::stopScreenRecording>;
constexpr auto cancelScreenRecording    = &core::makeMethod<&Self::cancelScreenRecording>;
constexpr auto getScreenRecordingUrl    = &core::makeMethod<&Self::getScreenRecordingUrl>;
constexpr auto checkRecordingPermission = &core::makeMethod<&Self::checkRecordingPermission>;
// clang-format on

SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_FUNC(startScreenRecording)
SE_BIND_FUNC(stopScreenRecording)
SE_BIND_FUNC(cancelScreenRecording)
SE_BIND_FUNC(getScreenRecordingUrl)
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

    EE_JSB_DEFINE_FUNCTION(cls, startScreenRecording);
    EE_JSB_DEFINE_FUNCTION(cls, stopScreenRecording);
    EE_JSB_DEFINE_FUNCTION(cls, cancelScreenRecording);
    EE_JSB_DEFINE_FUNCTION(cls, getScreenRecordingUrl);
    EE_JSB_DEFINE_FUNCTION(cls, checkRecordingPermission);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace recorder
} // namespace ee
