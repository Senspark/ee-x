//
//  Created by Zinge on 7/4/16.
//
//

#include "ee/recorder/RecorderBridge.hpp"
#include "ee/core/MessageBridge.hpp"
#include "ee/core/Utils.hpp"

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

Self::Recorder()
    : bridge_(MessageBridge::getInstance()) {
    //
}

Self::~Recorder() {
    //
}

void Self::startScreenRecording() {
    bridge_.call(k__startScreenRecording);
}

void Self::stopScreenRecording() {
    bridge_.call(k__stopScreenRecording);
}

void Self::cancelScreenRecording() {
    bridge_.call(k__cancelScreenRecording);
}

std::string Self::getScreenRecordingUrl() const {
    return bridge_.call(k__getScreenRecordingUrl);
}

bool Self::checkRecordingPermission() const {
    auto response = bridge_.call(k__checkRecordingPermission);
    return core::toBool(response);
}
} // namespace recorder
} // namespace ee
