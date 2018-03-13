//
//  Created by Zinge on 7/4/16.
//
//

#include "ee/recorder/RecorderBridge.hpp"
#include "ee/core/MessageBridge.hpp"
#include "ee/core/Utils.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace recorder {
namespace {
// clang-format off
constexpr auto k__startScreenRecording     = "Recorder_startScreenRecording";
constexpr auto k__stopScreenRecording      = "Recorder_stopScreenRecording";
constexpr auto k__cancelScreenRecording    = "Recorder_cancelScreenRecording";
constexpr auto k__getScreenRecordingUrl    = "Recorder_getScreenRecordingUrl";
constexpr auto k__checkRecordingPermission = "Recorder_checkRecordingPermission";
// clang-format on
} // namespace

using Self = Recorder;

Self::Recorder() {
    //
}

Self::~Recorder() {
    //
}

void Self::startScreenRecording() {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__startScreenRecording);
}

void Self::stopScreenRecording() {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__stopScreenRecording);
}

void Self::cancelScreenRecording() {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__cancelScreenRecording);
}

std::string Self::getScreenRecordingUrl() const {
    auto&& bridge = MessageBridge::getInstance();
    return bridge.call(k__getScreenRecordingUrl);
}

bool Self::checkRecordingPermission() const {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__checkRecordingPermission);
    return core::toBool(response);
}
} // namespace recorder
} // namespace ee
