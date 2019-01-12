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

se::Class* __jsb_Recorder_class = nullptr;

const auto jsb_Recorder_finalize = &core::jsb_finalize<Recorder>;
const auto jsb_Recorder_constructor = &core::jsb_constructor<Recorder>;

const auto jsb_Recorder_startScreenRecording =
    &core::jsb_method_call<Recorder, &Recorder::startScreenRecording>;
const auto jsb_Recorder_stopScreenRecording =
    &core::jsb_method_call<Recorder, &Recorder::stopScreenRecording>;
const auto jsb_Recorder_cancelScreenRecording =
    &core::jsb_method_call<Recorder, &Recorder::cancelScreenRecording>;

const auto jsb_Recorder_getScreenRecordingUrl =
    &core::jsb_method_get<Recorder, &Recorder::getScreenRecordingUrl,
                          const std::string&>;
const auto jsb_Recorder_checkRecordingPermission =
    &core::jsb_method_get<Recorder, &Recorder::checkRecordingPermission,
                          const bool>;

SE_BIND_FINALIZE_FUNC(jsb_Recorder_finalize)
SE_BIND_CTOR(jsb_Recorder_constructor, __jsb_Recorder_class,
             jsb_Recorder_finalize)

SE_BIND_FUNC(jsb_Recorder_startScreenRecording)
SE_BIND_FUNC(jsb_Recorder_stopScreenRecording)
SE_BIND_FUNC(jsb_Recorder_cancelScreenRecording)

SE_BIND_FUNC(jsb_Recorder_getScreenRecordingUrl)
SE_BIND_FUNC(jsb_Recorder_checkRecordingPermission)

bool register_recorder_bridge_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* recorderObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("recorder", eeObj, &recorderObj);

    auto cls = se::Class::create("Recorder", recorderObj, nullptr,
                                 _SE(jsb_Recorder_constructor));
    cls->defineFinalizeFunction(_SE(jsb_Recorder_finalize));

    cls->defineFunction("startScreenRecording",
                        _SE(jsb_Recorder_startScreenRecording));
    cls->defineFunction("stopScreenRecording",
                        _SE(jsb_Recorder_stopScreenRecording));
    cls->defineFunction("cancelScreenRecording",
                        _SE(jsb_Recorder_cancelScreenRecording));

    cls->defineFunction("getScreenRecordingUrl",
                        _SE(jsb_Recorder_getScreenRecordingUrl));
    cls->defineFunction("checkRecordingPermission",
                        _SE(jsb_Recorder_checkRecordingPermission));

    cls->install();

    JSBClassType::registerClass<Notification>(cls);

    __jsb_Recorder_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace recorder
} // namespace ee
