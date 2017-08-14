#include <thread>

#include "ee/ironsrc/IronSrcBridge.hpp"
#include "ee/core/internal/MessageBridge.hpp"

#include <nlohmann/json.hpp>

namespace ee {
namespace ironsrc {
IronSrc::IronSrc() {
    _callback = nullptr;
    core::MessageBridge::getInstance().registerHandler(
        std::bind(&IronSrc::doCallBack, this, std::placeholders::_1),
        "__IronSrcAds_callback");
}

IronSrc::~IronSrc() {
    core::MessageBridge::getInstance().deregisterHandler(
        "__IronSrcAds_callback");
}

std::string IronSrc::doCallBack(const std::string& msg) const {
    if (_callback) {
        
        nlohmann::json json = nlohmann::json::parse(msg);
        
        auto code = static_cast<IronSrcAdsResultCode>(json["code"].get<int>());
        std::string message = json["placement"];
        
        _callback(code, message);
    }

    return "";
}    

void IronSrc::initIronSrc(const std::string& gameID)
{
    nlohmann::json json;
    json["GameID"] = gameID;
    core::MessageBridge::getInstance().call("k__IronSrc_initIronSrc", json.dump());
}

bool IronSrc::isAdsReady(const std::string& placementID)
{
    nlohmann::json json;
    json["PlacementID"] = placementID;
    std::string result = core::MessageBridge::getInstance().call("k__IronSrc_isAdsReady", json.dump());
    
    return result.compare("true") == 0;
}

void IronSrc::showAds(const std::string& placementID)
{
    nlohmann::json json;
    json["PlacementID"] = placementID;
    core::MessageBridge::getInstance().call("k__IronSrc_showAds", json.dump());
}

} // namespace IronSrc
} // namespace ee
