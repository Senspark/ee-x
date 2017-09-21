//
//  ALovinAds.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include "ee/alovinads/ALovinAdsBridge.hpp"
#include "ee/core/internal/MessageBridge.hpp"

#include <nlohmann/json.hpp>

namespace ee {
namespace alovinads {
ALovinAds::ALovinAds() {
    callback_ = nullptr;
    core::MessageBridge::getInstance().registerHandler(
        std::bind(&ALovinAds::doCallBack, this, std::placeholders::_1),
        "__ALovinAds_callback");
}

ALovinAds::~ALovinAds() {
    core::MessageBridge::getInstance().deregisterHandler("__ALovinAds_callback");
}

std::string ALovinAds::doCallBack(const std::string& msg) const {
    if (callback_) {
        auto json = nlohmann::json::parse(msg);

        auto code = static_cast<ALovinAdsResultCode>(json["code"].get<int>());
        std::string message = json["placement"];

        callback_(code, message);
    }

    return "";
}

void ALovinAds::initALovinAds(const std::string& gameId) {
    nlohmann::json json;
    json["GameID"] = gameId;
    core::MessageBridge::getInstance().call("k__ALovinads_initALovinAds",
                                            json.dump());
}

bool ALovinAds::isAdsReady(const std::string& placementId) {
    nlohmann::json json;
    json["PlacementID"] = placementId;
    auto result = core::MessageBridge::getInstance().call(
        "k__ALovinads_isAdsReady", json.dump());
    return result == "true";
}

void ALovinAds::showAds(const std::string& placementId) {
    nlohmann::json json;
    json["PlacementID"] = placementId;
    core::MessageBridge::getInstance().call("k__ALovinads_showAds", json.dump());
}
} // namespace ALovinads
} // namespace ee
