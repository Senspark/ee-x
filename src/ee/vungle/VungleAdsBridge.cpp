#include "ee/vungle/VungleAdsBridge.hpp"
#include "ee/core/internal/MessageBridge.hpp"

#include <nlohmann/json.hpp>

namespace ee {
namespace vungle {
VungleAds::VungleAds() {
    callback_ = nullptr;
    core::MessageBridge::getInstance().registerHandler(
        std::bind(&VungleAds::doCallBack, this, std::placeholders::_1),
        "__VungleAds_callback");
}

VungleAds::~VungleAds() {
    core::MessageBridge::getInstance().deregisterHandler(
        "__VungleAds_callback");
}

std::string VungleAds::doCallBack(const std::string& msg) const {
    if (callback_) {
        auto json = nlohmann::json::parse(msg);

        auto code = static_cast<VungleAdsResultCode>(json["code"].get<int>());
        std::string message = json["placement"];

        callback_(code, message);
    }

    return "";
}

void VungleAds::initVungleAds(const std::string& gameId, const std::string& placementIds) {
    nlohmann::json json;
    json["GameID"] = gameId;
    json["PlacementIDs"] = placementIds;
    core::MessageBridge::getInstance().call("k__VungleAds_initVungleAds",
                                            json.dump());
}

bool VungleAds::isAdsReady(const std::string& placementId) {
    nlohmann::json json;
    json["PlacementID"] = placementId;
    auto result = core::MessageBridge::getInstance().call(
        "k__VungleAds_isAdReady", json.dump());
    return result == "true";
}

void VungleAds::showAds(const std::string& placementId) {
    nlohmann::json json;
    json["PlacementID"] = placementId;
    core::MessageBridge::getInstance().call("k__VungleAds_showAds", json.dump());
}
} // namespace VungleAds
} // namespace ee
