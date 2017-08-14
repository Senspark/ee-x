#include "ee/ironsrc/IronSrcBridge.hpp"
#include "ee/core/internal/MessageBridge.hpp"

#include <nlohmann/json.hpp>

namespace ee {
namespace ironsrc {
IronSrc::IronSrc() {
    callback_ = nullptr;
    core::MessageBridge::getInstance().registerHandler(
        std::bind(&IronSrc::doCallBack, this, std::placeholders::_1),
        "__IronSrcAds_callback");
}

IronSrc::~IronSrc() {
    core::MessageBridge::getInstance().deregisterHandler(
        "__IronSrcAds_callback");
}

std::string IronSrc::doCallBack(const std::string& msg) const {
    if (callback_) {
        auto json = nlohmann::json::parse(msg);

        auto code = static_cast<IronSrcAdsResultCode>(json["code"].get<int>());
        std::string message = json["placement"];

        callback_(code, message);
    }

    return "";
}

void IronSrc::initIronSrc(const std::string& gameId) {
    nlohmann::json json;
    json["GameID"] = gameId;
    core::MessageBridge::getInstance().call("k__IronSrc_initIronSrc",
                                            json.dump());
}

bool IronSrc::isAdsReady(const std::string& placementId) {
    nlohmann::json json;
    json["PlacementID"] = placementId;
    auto result = core::MessageBridge::getInstance().call(
        "k__IronSrc_isAdsReady", json.dump());
    return result == "true";
}

void IronSrc::showAds(const std::string& placementId) {
    nlohmann::json json;
    json["PlacementID"] = placementId;
    core::MessageBridge::getInstance().call("k__IronSrc_showAds", json.dump());
}
} // namespace ironsrc
} // namespace ee
