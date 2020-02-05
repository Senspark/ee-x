//
//  Created by Zinge on 7/4/16.
//
//

#include "ee/recorder/RecorderBridge.hpp"

#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>

namespace ee {
namespace recorder {
namespace {
// clang-format off
constexpr auto k__isSupported              = "Recorder_isSupported";
constexpr auto k__startRecording           = "Recorder_startRecording";
constexpr auto k__stopRecording            = "Recorder_stopRecording";
constexpr auto k__cancelRecording          = "Recorder_cancelRecording";
constexpr auto k__getRecordingUrl          = "Recorder_getRecordingUrl";
constexpr auto k__checkRecordingPermission = "Recorder_checkRecordingPermission";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge()
    : bridge_(MessageBridge::getInstance()) {
    //
}

Self::~Bridge() {
    //
}

bool Self::isSupported() const {
    auto&& response = bridge_.call(k__isSupported);
    return core::toBool(response);
}

void Self::startRecording() {
    bridge_.call(k__startRecording);
}

void Self::stopRecording() {
    bridge_.call(k__stopRecording);
}

void Self::cancelRecording() {
    bridge_.call(k__cancelRecording);
}

std::string Self::getRecordingUrl() const {
    return bridge_.call(k__getRecordingUrl);
}

bool Self::checkRecordingPermission() const {
    auto response = bridge_.call(k__checkRecordingPermission);
    return core::toBool(response);
}
} // namespace recorder
} // namespace ee
