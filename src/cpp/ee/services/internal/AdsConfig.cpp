#include "ee/services/internal/AdsConfig.hpp"

#include <cassert>

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace services {
std::shared_ptr<NetworkConfig>
NetworkConfig::parseImpl(const nlohmann::json& node) {
    auto&& network = node["network"];
    if (network == "ad_mob") {
        return std::make_shared<AdMobConfig>(node);
    }
    if (network == "app_lovin") {
        return std::make_shared<AppLovinConfig>(node);
    }
    if (network == "facebook_ads") {
        return std::make_shared<FacebookAdsConfig>(node);
    }
    if (network == "iron_source") {
        return std::make_shared<IronSourceConfig>(node);
    }
    if (network == "unity_ads") {
        return std::make_shared<UnityAdsConfig>(node);
    }
    if (network == "vungle") {
        return std::make_shared<VungleConfig>(node);
    }
    assert(false);
    return nullptr;
}

Network NetworkConfig::network() const {
    return network_;
}

AdMobConfig::AdMobConfig(const nlohmann::json& node) {
    network_ = Network::AdMob;
}

AppLovinConfig::AppLovinConfig(const nlohmann::json& node) {
    network_ = Network::AppLovin;
    appId_ = node["app_id"];
}

const std::string& AppLovinConfig::appId() const {
    return appId_;
}

FacebookAdsConfig::FacebookAdsConfig(const nlohmann::json& node) {
    network_ = Network::FacebookAds;
}

IronSourceConfig::IronSourceConfig(const nlohmann::json& node) {
    network_ = Network::IronSource;
    appId_ = node["app_id"];
}

const std::string& IronSourceConfig::appId() const {
    return appId_;
}

UnityAdsConfig::UnityAdsConfig(const nlohmann::json& node) {
    network_ = Network::UnityAds;
    appId_ = node["app_id"];
}

const std::string& UnityAdsConfig::appId() const {
    return appId_;
}

VungleConfig::VungleConfig(const nlohmann::json& node) {
    network_ = Network::Vungle;
    appId_ = node["app_id"];
}

const std::string& VungleConfig::appId() const {
    return appId_;
}

std::shared_ptr<AdConfig> AdConfig::parseImpl(const nlohmann::json& node) {
    auto&& format = node["format"];
    if (format == "banner") {
        return std::make_shared<BannerConfig>(node);
    }
    if (format == "app_open") {
        return std::make_shared<AppOpenConfig>(node);
    }
    if (format == "interstitial") {
        return std::make_shared<InterstitialConfig>(node);
    }
    if (format == "rewarded") {
        return std::make_shared<RewardedConfig>(node);
    }
    assert(false);
    return nullptr;
}

AdFormat AdConfig::adFormat() const {
    return adFormat_;
}

const std::shared_ptr<AdInstanceConfig>& AdConfig::instance() const {
    return instance_;
}

BannerConfig::BannerConfig(const nlohmann::json& node) {
    adFormat_ = AdFormat::Banner;
    instance_ = AdInstanceConfig::parseImpl(node["instance"]);
}

AppOpenConfig::AppOpenConfig(const nlohmann::json& node) {
    adFormat_ = AdFormat::AppOpen;
    interval_ = node.value("interval", 0);
    instance_ = AdInstanceConfig::parseImpl(node["instance"]);
}

int AppOpenConfig::interval() const {
    return interval_;
}

InterstitialConfig::InterstitialConfig(const nlohmann::json& node) {
    adFormat_ = AdFormat::Interstitial;
    interval_ = node.value("interval", 0);
    instance_ = AdInstanceConfig::parseImpl(node["instance"]);
}

int InterstitialConfig::interval() const {
    return interval_;
}

RewardedConfig::RewardedConfig(const nlohmann::json& node) {
    adFormat_ = AdFormat::Rewarded;
    instance_ = AdInstanceConfig::parseImpl(node["instance"]);
}

std::shared_ptr<AdInstanceConfig>
AdInstanceConfig::parseImpl(const nlohmann::json& node) {
    if (node.is_array()) {
        return std::make_shared<WaterfallInstanceConfig>(node);
    }
    return std::make_shared<SingleInstanceConfig>(node);
}

SingleInstanceConfig::SingleInstanceConfig(const nlohmann::json& node) {
    std::map<std::string, Network> networks = {
        {"ad_mob", Network::AdMob},
        {"app_lovin", Network::AppLovin},
        {"facebook_ads", Network::FacebookAds},
        {"iron_source", Network::IronSource},
        {"unity_ads", Network::UnityAds},
        {"vungle", Network::Vungle},
    };
    auto&& network = node["network"];
    network_ = networks[network];
    id_ = node.value("id", "");
}

Network SingleInstanceConfig::network() const {
    return network_;
}

const std::string& SingleInstanceConfig::id() const {
    return id_;
}

WaterfallInstanceConfig::WaterfallInstanceConfig(const nlohmann::json& node) {
    for (auto&& [key, value] : node.items()) {
        instances_.push_back(parseImpl(value));
    }
}

const std::vector<std::shared_ptr<AdInstanceConfig>>&
WaterfallInstanceConfig::instances() const {
    return instances_;
}

using Self = AdsConfig;

std::shared_ptr<AdsConfig> Self::parse(const std::string& text) {
    auto&& node = nlohmann::json::parse(text);
    return parse(node);
}

std::shared_ptr<AdsConfig> Self::parse(const nlohmann::json& node) {
    auto result = std::make_shared<AdsConfig>();
    for (auto&& [key, value] : node["networks"].items()) {
        result->networks_.push_back(NetworkConfig::parseImpl(value));
    }
    for (auto&& [key, value] : node["ads"].items()) {
        result->ads_.push_back(AdConfig::parseImpl(value));
    }
    return result;
}

const std::vector<std::shared_ptr<NetworkConfig>>& Self::networks() const {
    return networks_;
}

const std::vector<std::shared_ptr<AdConfig>>& Self::ads() const {
    return ads_;
}
} // namespace services
} // namespace ee
