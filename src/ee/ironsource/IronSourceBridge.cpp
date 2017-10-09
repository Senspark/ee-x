#include "ee/ironsource/IronSourceBridge.hpp"
#include "ee/core/internal/MessageBridge.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace ironsource {
using Self = IronSource;

namespace {
// clang-format off
constexpr auto k__initialize        = "IronSource_initialize";
constexpr auto k__cppCallback       = "IronSource_cppCallback";
constexpr auto k__showRewardedVideo = "IronSource_showRewardedVideo";
// clang-format on
} // namespace

Self::IronSource() {
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

Self::~IronSource() {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.deregisterHandler(k__cppCallback);
}

void Self::initialize(const std::string& gameId) {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__initialize, gameId);
}

bool Self::showRewardedVideo(const std::string& placementId) {
    auto&& bridge = core::MessageBridge::getInstance();
    auto result = bridge.call(k__showRewardedVideo, placementId);
    return result == "true";
}
} // namespace ironsource
} // namespace ee
