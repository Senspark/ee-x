#include "ee/services/internal/AdsConfig.hpp"

#include <cassert>

#include <ee/ad_mob/AdMobBannerAdSize.hpp>
#include <ee/ad_mob/IAdMobBridge.hpp>
#include <ee/ads/MultiAdView.hpp>
#include <ee/ads/MultiFullScreenAd.hpp>
#include <ee/app_lovin/IAppLovinBridge.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/Task.hpp>
#include <ee/facebook_ads/FacebookBannerAdSize.hpp>
#include <ee/facebook_ads/IFacebookAdsBridge.hpp>
#include <ee/iron_source/IIronSourceBridge.hpp>
#include <ee/nlohmann/json.hpp>
#include <ee/unity_ads/IUnityAdsBridge.hpp>
#include <ee/vungle/IVungleBridge.hpp>

namespace ee {
namespace services {
std::shared_ptr<NetworkConfig>
NetworkConfig::parse(const nlohmann::json& node) {
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

AdMobConfig::AdMobConfig(const nlohmann::json& node) {}

Task<> AdMobConfig::initialize() {
    plugin_ = PluginManager::createPlugin<IAdMob>();
    co_await plugin_->initialize();
}

Network AdMobConfig::network() const {
    return Network::AdMob;
}

std::shared_ptr<IAd> AdMobConfig::createAd(AdFormat format,
                                           const std::string& id) {
    switch (format) {
    case AdFormat::Banner:
        return plugin_->createBannerAd(id, AdMobBannerAdSize::Normal);
    case AdFormat::AppOpen:
        return plugin_->createAppOpenAd(id);
    case AdFormat::Interstitial:
        return plugin_->createInterstitialAd(id);
    case AdFormat::Rewarded:
        return plugin_->createRewardedAd(id);
    default:
        assert(false);
        return nullptr;
    }
}

AppLovinConfig::AppLovinConfig(const nlohmann::json& node) {
    appId_ = node["app_id"];
}

Task<> AppLovinConfig::initialize() {
    plugin_ = PluginManager::createPlugin<IAppLovin>();
    co_await plugin_->initialize(appId_);
}

Network AppLovinConfig::network() const {
    return Network::AppLovin;
}

std::shared_ptr<IAd> AppLovinConfig::createAd(AdFormat format,
                                              const std::string& id) {
    switch (format) {
    case AdFormat::Rewarded:
        return plugin_->createRewardedAd();
    default:
        assert(false);
        return nullptr;
    }
}

FacebookAdsConfig::FacebookAdsConfig(const nlohmann::json& node) {}

Task<> FacebookAdsConfig::initialize() {
    plugin_ = PluginManager::createPlugin<IFacebookAds>();
    co_await plugin_->initialize();
}

Network FacebookAdsConfig::network() const {
    return Network::FacebookAds;
}

std::shared_ptr<IAd> FacebookAdsConfig::createAd(AdFormat format,
                                                 const std::string& id) {
    switch (format) {
    case AdFormat::Banner:
        return plugin_->createBannerAd(id,
                                       FacebookBannerAdSize::BannerHeight50);
    case AdFormat::Interstitial:
        return plugin_->createInterstitialAd(id);
    case AdFormat::Rewarded:
        return plugin_->createRewardedAd(id);
    default:
        assert(false);
        return nullptr;
    }
}

IronSourceConfig::IronSourceConfig(const nlohmann::json& node) {
    appId_ = node["app_id"];
}

Task<> IronSourceConfig::initialize() {
    plugin_ = PluginManager::createPlugin<IIronSource>();
    co_await plugin_->initialize(appId_);
}

Network IronSourceConfig::network() const {
    return Network::IronSource;
}

std::shared_ptr<IAd> IronSourceConfig::createAd(AdFormat format,
                                                const std::string& id) {
    switch (format) {
    case AdFormat::Interstitial:
        return plugin_->createInterstitialAd(id);
    case AdFormat::Rewarded:
        return plugin_->createRewardedAd(id);
    default:
        assert(false);
        return nullptr;
    }
}

UnityAdsConfig::UnityAdsConfig(const nlohmann::json& node) {
    appId_ = node["app_id"];
}

Task<> UnityAdsConfig::initialize() {
    plugin_ = PluginManager::createPlugin<IUnityAds>();
    co_await plugin_->initialize(appId_, false);
}

Network UnityAdsConfig::network() const {
    return Network::UnityAds;
}

std::shared_ptr<IAd> UnityAdsConfig::createAd(AdFormat format,
                                              const std::string& id) {
    switch (format) {
    case AdFormat::Interstitial:
        return plugin_->createInterstitialAd(id);
    case AdFormat::Rewarded:
        return plugin_->createRewardedAd(id);
    default:
        assert(false);
        return nullptr;
    }
}

VungleConfig::VungleConfig(const nlohmann::json& node) {
    appId_ = node["app_id"];
}

Task<> VungleConfig::initialize() {
    plugin_ = PluginManager::createPlugin<IVungle>();
    co_await plugin_->initialize(appId_);
}

Network VungleConfig::network() const {
    return Network::Vungle;
}

std::shared_ptr<IAd> VungleConfig::createAd(AdFormat format,
                                            const std::string& id) {
    switch (format) {
    case AdFormat::Rewarded:
        return plugin_->createRewardedAd(id);
    default:
        assert(false);
        return nullptr;
    }
}

std::shared_ptr<AdConfig> AdConfig::parse(const nlohmann::json& node) {
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

BannerConfig::BannerConfig(const nlohmann::json& node) {
    instance_ = AdInstanceConfig<IAdView>::parse<MultiAdView>(AdFormat::Banner,
                                                              node["instance"]);
}

AdFormat BannerConfig::format() const {
    return AdFormat::Banner;
}

AppOpenConfig::AppOpenConfig(const nlohmann::json& node) {
    interval_ = node.value("interval", 0);
    instance_ = AdInstanceConfig<IFullScreenAd>::parse<MultiFullScreenAd>(
        AdFormat::AppOpen, node["instance"]);
}

AdFormat AppOpenConfig::format() const {
    return AdFormat::AppOpen;
}

int AppOpenConfig::interval() const {
    return interval_;
}

InterstitialConfig::InterstitialConfig(const nlohmann::json& node) {
    interval_ = node.value("interval", 0);
    instance_ = AdInstanceConfig<IFullScreenAd>::parse<MultiFullScreenAd>(
        AdFormat::Interstitial, node["instance"]);
}

AdFormat InterstitialConfig::format() const {
    return AdFormat::Interstitial;
}

int InterstitialConfig::interval() const {
    return interval_;
}

RewardedConfig::RewardedConfig(const nlohmann::json& node) {
    instance_ = AdInstanceConfig<IFullScreenAd>::parse<MultiFullScreenAd>(
        AdFormat::Rewarded, node["instance"]);
}

AdFormat RewardedConfig::format() const {
    return AdFormat::Rewarded;
}

template <class Ad>
template <class MultiAd>
std::shared_ptr<AdInstanceConfig<Ad>>
AdInstanceConfig<Ad>::parse(AdFormat format, const nlohmann::json& node) {
    if (node.is_array()) {
        return std::make_shared<WaterfallInstanceConfig<Ad, MultiAd>>(format,
                                                                      node);
    }
    return std::make_shared<SingleInstanceConfig<Ad>>(format, node);
}

template <class Ad>
SingleInstanceConfig<Ad>::SingleInstanceConfig(AdFormat format,
                                               const nlohmann::json& node) {
    format_ = format;
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

template <class Ad>
std::shared_ptr<Ad>
SingleInstanceConfig<Ad>::createAd(INetworkConfigManager& manager) const {
    auto ad = manager.createAd(network_, format_, id_);
    return std::dynamic_pointer_cast<Ad>(ad);
}

template <class Ad, class MultiAd>
WaterfallInstanceConfig<Ad, MultiAd>::WaterfallInstanceConfig(
    AdFormat format, const nlohmann::json& node) {
    for (auto&& [key, value] : node.items()) {
        instances_.push_back(
            AdInstanceConfig<Ad>::template parse<MultiAd>(format, value));
    }
}

template <class Ad, class MultiAd>
std::shared_ptr<Ad> WaterfallInstanceConfig<Ad, MultiAd>::createAd(
    INetworkConfigManager& manager) const {
    auto ad = std::make_shared<MultiAd>();
    for (auto&& instance : instances_) {
        ad->addItem(instance->createAd(manager));
    }
    return ad;
}

using Self = AdsConfig;

std::shared_ptr<AdsConfig> Self::parse(const std::string& text) {
    auto&& node = nlohmann::json::parse(text);
    return parse(node);
}

std::shared_ptr<AdsConfig> Self::parse(const nlohmann::json& node) {
    auto result = std::make_shared<AdsConfig>();
    for (auto&& [key, value] : node["networks"].items()) {
        auto network = NetworkConfig::parse(value);
        result->networks_[network->network()] = network;
    }
    for (auto&& [key, value] : node["ads"].items()) {
        result->ads_.push_back(AdConfig::parse(value));
    }
    return result;
}

Task<> Self::initialize() {
    for (auto&& [key, value] : networks_) {
        co_await value->initialize();
    }
}

std::shared_ptr<IAd> Self::createAd(AdFormat format) {
    for (auto&& ad : ads_) {
        if (ad->format() == format) {
            return ad->createAd(*this);
        }
    }
    return nullptr;
}
} // namespace services
} // namespace ee
