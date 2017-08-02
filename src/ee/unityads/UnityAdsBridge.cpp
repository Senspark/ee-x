//
//  UnityAds.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include <thread>

#include "ee/unityads/UnityAdsBridge.hpp"
//#include "ee/core/Logger.hpp"
//#include "ee/core/LogLevel.hpp"
#include "ee/core/internal/MessageBridge.hpp"

#include <nlohmann/json.hpp>

namespace ee {
namespace unitylib {
UnityAds::UnityAds() {
    _callback = nullptr;
    core::MessageBridge::getInstance().registerHandler(
        std::bind(&UnityAds::doCallBack, this, std::placeholders::_1),
        "__UnityAds_callback");
}

UnityAds::~UnityAds() {
    core::MessageBridge::getInstance().deregisterHandler(
        "__UnityAds_callback");
}

std::string UnityAds::doCallBack(const std::string& msg) const {
    if (_callback) {
        
        nlohmann::json json = nlohmann::json::parse(msg);
        
        auto code = static_cast<UnityAdsResultCode>(json["code"].get<int>());
        std::string message = json["placement"];
        
        _callback(code, message);
    }

    return "";
}    

void UnityAds::initUnityAds(const std::string& gameID)
{
    nlohmann::json json;
    json["GameID"] = gameID;
    core::MessageBridge::getInstance().call("k__unityads_initUnityAds", json.dump());
}

bool UnityAds::isAdsReady(const std::string& placementID)
{
    nlohmann::json json;
    json["PlacementID"] = placementID;
    std::string result = core::MessageBridge::getInstance().call("k__unityads_isAdsReady", json.dump());
    
    return result.compare("true") == 0;
}

void UnityAds::showAds(const std::string& placementID)
{
    nlohmann::json json;
    json["PlacementID"] = placementID;
    core::MessageBridge::getInstance().call("k__unityads_showAds", json.dump());
}

} // namespace crashlytics
} // namespace ee
