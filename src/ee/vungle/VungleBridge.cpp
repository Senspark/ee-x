#include "ee/vungle/VungleBridge.hpp"
#include "ee/core/internal/MessageBridge.hpp"

#include <nlohmann/json.hpp>

namespace ee {
namespace vungle {
using Self = Vungle;

namespace {
// clang-format off
constexpr auto k__VungleAds_initVungleAds   = "k__VungleAds_initVungleAds";
constexpr auto k__VungleAds_isAdReady       = "k__VungleAds_isAdReady";
constexpr auto k__VungleAds_showAds         = "k__VungleAds_showAds";
constexpr auto k__Vungle_cpp_callback       = "__VungleAds_callback";
// clang-format on
} // namespace

Self::Vungle() {
    callback_ = nullptr;
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.registerHandler(
        std::bind(&Self::doCallBack, this, std::placeholders::_1),
        k__Vungle_cpp_callback);
}

Self::~Vungle() {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.deregisterHandler(k__Vungle_cpp_callback);
}

std::string Self::doCallBack(const std::string& msg) const {
    if (callback_) {
        auto json = nlohmann::json::parse(msg);

        auto code = static_cast<VungleAdsResultCode>(json["code"].get<int>());
        std::string message = json["placement"];

        callback_(code, message);
    }

    return "";
}

void Self::initialize(const std::string& gameId,
                      const std::vector<std::string>& placementIds) {
    nlohmann::json json;

    json["GameID"] = gameId;
    json["PlacementIDs"] = placementIds;

    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__VungleAds_initVungleAds, json.dump());
}

bool Self::isAdsReady(const std::string& placementId) {
    auto&& bridge = core::MessageBridge::getInstance();
    auto result = bridge.call(k__VungleAds_isAdReady, placementId);
    return result == "true";
}

void Self::showAds(const std::string& placementId) {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__VungleAds_showAds, placementId);
}
} // namespace vungle
} // namespace ee
