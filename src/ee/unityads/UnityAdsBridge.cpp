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
UnityAds::UnityAds() {
    callback_ = nullptr;
    core::MessageBridge::getInstance().registerHandler(
        std::bind(&UnityAds::doCallBack, this, std::placeholders::_1),
        "__UnityAds_callback");
}

UnityAds::~UnityAds() {
    core::MessageBridge::getInstance().deregisterHandler("__UnityAds_callback");
}

std::string UnityAds::doCallBack(const std::string& msg) const {
    if (callback_) {
        auto json = nlohmann::json::parse(msg);

        auto code = static_cast<UnityAdsResultCode>(json["code"].get<int>());
        std::string message = json["placement"];

        callback_(code, message);
    }

    return "";
}

void UnityAds::initUnityAds(const std::string& gameId) {
    nlohmann::json json;
    json["GameID"] = gameId;
    core::MessageBridge::getInstance().call("k__unityads_initUnityAds",
                                            json.dump());
}

bool UnityAds::isAdsReady(const std::string& placementId) {
    nlohmann::json json;
    json["PlacementID"] = placementId;
    auto result = core::MessageBridge::getInstance().call(
        "k__unityads_isAdsReady", json.dump());
    return result == "true";
}

void UnityAds::showAds(const std::string& placementId) {
    nlohmann::json json;
    json["PlacementID"] = placementId;
    core::MessageBridge::getInstance().call("k__unityads_showAds", json.dump());
}
} // namespace unityads
} // namespace ee
