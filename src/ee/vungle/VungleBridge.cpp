#include "ee/vungle/VungleBridge.hpp"
#include "ee/core/internal/MessageBridge.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace vungle {
using Self = Vungle;

namespace {
// clang-format off
constexpr auto k__initialize        = "Vungle_initialize";
constexpr auto k__showRewardedVideo = "Vungle_showRewardedVideo";
constexpr auto k__cppCallback       = "Vungle_cppCallback";
// clang-format on
} // namespace

const std::string Self::DefaultPlacementId = "Vungle_placementId";

Self::Vungle() {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            auto result = json["result"].get<bool>();
            invokeRewardedVideoCallback(result, DefaultPlacementId);
            return "";
        },
        k__cppCallback);
}

Self::~Vungle() {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.deregisterHandler(k__cppCallback);
}

void Self::initialize(const std::string& gameId) {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__initialize, gameId);
}

bool Self::showRewardedVideo(const std::string& placementId) {
    if (placementId != DefaultPlacementId) {
        return false;
    }
    auto&& bridge = core::MessageBridge::getInstance();
    auto result = bridge.call(k__showRewardedVideo);
    return result == "true";
}
} // namespace vungle
} // namespace ee
