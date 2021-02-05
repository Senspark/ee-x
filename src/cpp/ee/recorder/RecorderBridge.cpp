//
//  Created by Zinge on 7/4/16.
//
//

#include "ee/recorder/RecorderBridge.hpp"

#include <ee/core/IMessageBridge.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/PluginManagerUtils.hpp>

namespace ee {
namespace recorder {
namespace {
// clang-format off
const std::string kPrefix            = "RecorderBridge";
const auto kIsSupported              = kPrefix + "IsSupported";
const auto kStartRecording           = kPrefix + "StartRecording";
const auto kStopRecording            = kPrefix + "StopRecording";
const auto kCancelRecording          = kPrefix + "CancelRecording";
const auto kGetRecordingUrl          = kPrefix + "GetRecordingUrl";
const auto kCheckRecordingPermission = kPrefix + "CheckRecordingPermission";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge()
    : bridge_(PluginManager::getBridge()) {
    core::PluginManagerUtils::addPlugin("Recorder");
}

Self::~Bridge() = default;

void Self::destroy() {
    core::PluginManagerUtils::removePlugin("Recorder");
}

bool Self::isSupported() const {
    auto&& response = bridge_.call(kIsSupported);
    return core::toBool(response);
}

void Self::startRecording() {
    bridge_.call(kStartRecording);
}

void Self::stopRecording() {
    bridge_.call(kStopRecording);
}

void Self::cancelRecording() {
    bridge_.call(kCancelRecording);
}

std::string Self::getRecordingUrl() const {
    return bridge_.call(kGetRecordingUrl);
}

bool Self::checkRecordingPermission() const {
    auto response = bridge_.call(kCheckRecordingPermission);
    return core::toBool(response);
}
} // namespace recorder
} // namespace ee
