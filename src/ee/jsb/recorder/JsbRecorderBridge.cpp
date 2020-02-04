//
//  jsb_recorder_bridge.cpp
//  ee_x
//
//  Created by HanPx on 01/12/19.
//

#include "ee/jsb/recorder/JsbRecorderBridge.hpp"

#include <ee/Recorder.hpp>
#include <ee/jsb/JsbCore.hpp>

namespace ee {
namespace recorder {
namespace {
se::Class* clazz = nullptr;

using Self = Bridge;

// clang-format off
constexpr auto constructor              = &core::makeConstructor<Self>;
constexpr auto finalize                 = &core::makeFinalize<Self>;
constexpr auto isSupported              = &core::makeMethod<&Self::isSupported>;
constexpr auto startRecording           = &core::makeMethod<&Self::startRecording>;
constexpr auto stopRecording            = &core::makeMethod<&Self::stopRecording>;
constexpr auto cancelRecording          = &core::makeMethod<&Self::cancelRecording>;
constexpr auto getRecordingUrl          = &core::makeMethod<&Self::getRecordingUrl>;
constexpr auto checkRecordingPermission = &core::makeMethod<&Self::checkRecordingPermission>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(isSupported)
SE_BIND_FUNC(startRecording)
SE_BIND_FUNC(stopRecording)
SE_BIND_FUNC(cancelRecording)
SE_BIND_FUNC(getRecordingUrl)
SE_BIND_FUNC(checkRecordingPermission)
} // namespace

bool registerJsbBridge(se::Object* global) {
    auto scope = core::getPath(global, "ee");
    auto cls = se::Class::create("Recorder", scope, nullptr, _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, isSupported);
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
