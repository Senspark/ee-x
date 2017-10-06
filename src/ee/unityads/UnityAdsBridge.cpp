//
//  UnityAds.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include "ee/unityads/UnityAdsBridge.hpp"
#include "ee/core/internal/MessageBridge.hpp"

#include <nlohmann/json.hpp>

namespace ee {
namespace unityads {
using Self = UnityAds;

namespace {
// clang-format off
constexpr auto k__initialize        = "UnityAds_initialize";
constexpr auto k__cppCallback       = "UnityAds_cppCallback";
constexpr auto k__showRewardedVideo = "UnityAds_showRewardedVideo";
// clang-format on
} // namespace

Self::UnityAds() {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            auto result = json["result"].get<bool>();
            auto placementId = json["placementId"].get<std::string>();
            invokeRewardedVideoCallback(result, placementId);
            return "";
        },
        k__cppCallback);
}

Self::~UnityAds() {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.deregisterHandler(k__cppCallback);
}

void Self::initialize(const std::string& gameId) {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__initialize, gameId);
}

bool UnityAds::showRewardedVideo(const std::string& placementId) {
    auto&& bridge = core::MessageBridge::getInstance();
    auto result = bridge.call(k__showRewardedVideo, placementId);
    return result == "true";
}
} // namespace unityads
} // namespace ee
