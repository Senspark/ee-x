#include "ee/services/internal/AdsConfig.hpp"

#include <cassert>

#include <ee/ad_mob/AdMobBannerAdSize.hpp>
#include <ee/ad_mob/IAdMobBridge.hpp>
#include <ee/ads/MultiBannerAd.hpp>
#include <ee/ads/MultiFullScreenAd.hpp>
#include <ee/app_lovin/IAppLovinBridge.hpp>
#include <ee/cocos/CocosBannerAd.hpp>
#include <ee/core/Delay.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/WhenAny.hpp>
#include <ee/facebook_ads/FacebookBannerAdSize.hpp>
#include <ee/facebook_ads/IFacebookAdsBridge.hpp>
#include <ee/iron_source/IIronSourceBridge.hpp>
#include <ee/iron_source/IronSourceBannerAdSize.hpp>
#include <ee/nlohmann/json.hpp>
#include <ee/unity_ads/IUnityAdsBridge.hpp>
#include <ee/vungle/IVungleBridge.hpp>

#include "ee/services/internal/GenericAd.hpp"

namespace ee {
namespace services {
NetworkConfigManager::NetworkConfigManager(const nlohmann::json& node) {
    for (auto&& [key, value] : node["networks"].items()) {
        auto network = INetworkConfig::parse(value);
        networks_.push_back(network);
    }
}

Task<> NetworkConfigManager::initialize() {
    for (auto&& network : networks_) {
        co_await network->initialize();
    }
}

std::shared_ptr<IAd> NetworkConfigManager::createAd(Network network,
                                                    AdFormat format,
                                                    const std::string& id) {
    for (auto&& item : networks_) {
        if (item->network() == network) {
            return item->createAd(format, id);
        }
    }
    return nullptr;
}

std::shared_ptr<INetworkConfig>
INetworkConfig::parse(const nlohmann::json& node) {
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
    case AdFormat::Rectangle:
        return plugin_->createBannerAd(id, AdMobBannerAdSize::MediumRectangle);
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
    case AdFormat::Rectangle:
        return plugin_->createBannerAd(
            id, FacebookBannerAdSize::RectangleHeight250);
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
    case AdFormat::Banner:
        return plugin_->createBannerAd(id, IronSourceBannerAdSize::Banner);
    case AdFormat::Rectangle:
        return plugin_->createBannerAd(id, IronSourceBannerAdSize::Rectangle);
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
    timeOut_ = node.value("time_out", 30);
}

Task<> UnityAdsConfig::initialize() {
    plugin_ = PluginManager::createPlugin<IUnityAds>();
    co_await whenAny(
        [this]() -> Task<> { //
            co_await Delay(timeOut_);
        },
        [this]() -> Task<> { //
            co_await plugin_->initialize(appId_, false);
        });
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
    timeOut_ = node.value("time_out", 30);
}

Task<> VungleConfig::initialize() {
    plugin_ = PluginManager::createPlugin<IVungle>();
    co_await whenAny(
        [this]() -> Task<> { //
            co_await Delay(timeOut_);
        },
        [this]() -> Task<> { //
            co_await plugin_->initialize(appId_);
        });
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

AdConfigManager::AdConfigManager(
    const std::shared_ptr<INetworkConfigManager>& manager,
    const nlohmann::json& node)
    : manager_(manager) {
    for (auto&& [key, value] : node["ads"].items()) {
        auto ad = IAdConfig::parse(value);
        ads_.push_back(ad);
    }
}

std::shared_ptr<IAd> AdConfigManager::createAd(AdFormat format) {
    for (auto&& ad : ads_) {
        if (ad->format() == format) {
            return ad->createAd(manager_);
        }
    }
    return nullptr;
}

std::shared_ptr<IAdConfig> IAdConfig::parse(const nlohmann::json& node) {
    auto&& format = node["format"];
    if (format == "banner") {
        return std::make_shared<BannerConfig>(node);
    }
    if (format == "rect") {
        return std::make_shared<RectangleConfig>(node);
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
    instance_ = IAdInstanceConfig<IBannerAd>::parse<MultiBannerAd>(
        AdFormat::Banner, node["instance"]);
}

AdFormat BannerConfig::format() const {
    return AdFormat::Banner;
}

std::shared_ptr<IAd> BannerConfig::createAd(
    const std::shared_ptr<INetworkConfigManager>& manager) const {
    auto ad = instance_->createAd(manager);
    return std::make_shared<CocosBannerAd>(ad);
}

RectangleConfig::RectangleConfig(const nlohmann::json& node) {
    instance_ = IAdInstanceConfig<IBannerAd>::parse<MultiBannerAd>(
        AdFormat::Rectangle, node["instance"]);
}

AdFormat RectangleConfig::format() const {
    return AdFormat::Rectangle;
}

std::shared_ptr<IAd> RectangleConfig::createAd(
    const std::shared_ptr<INetworkConfigManager>& manager) const {
    auto ad = instance_->createAd(manager);
    return std::make_shared<CocosBannerAd>(ad);
}

AppOpenConfig::AppOpenConfig(const nlohmann::json& node) {
    interval_ = node.value("interval", 0);
    instance_ = IAdInstanceConfig<IFullScreenAd>::parse<MultiFullScreenAd>(
        AdFormat::AppOpen, node["instance"]);
}

AdFormat AppOpenConfig::format() const {
    return AdFormat::AppOpen;
}

std::shared_ptr<IAd> AppOpenConfig::createAd(
    const std::shared_ptr<INetworkConfigManager>& manager) const {
    auto ad = instance_->createAd(manager);
    return std::make_shared<GenericAd>(ad, interval_);
}

InterstitialConfig::InterstitialConfig(const nlohmann::json& node) {
    interval_ = node.value("interval", 0);
    instance_ = IAdInstanceConfig<IFullScreenAd>::parse<MultiFullScreenAd>(
        AdFormat::Interstitial, node["instance"]);
}

AdFormat InterstitialConfig::format() const {
    return AdFormat::Interstitial;
}

std::shared_ptr<IAd> InterstitialConfig::createAd(
    const std::shared_ptr<INetworkConfigManager>& manager) const {
    auto ad = instance_->createAd(manager);
    return std::make_shared<GenericAd>(ad, interval_);
}

RewardedConfig::RewardedConfig(const nlohmann::json& node) {
    instance_ = IAdInstanceConfig<IFullScreenAd>::parse<MultiFullScreenAd>(
        AdFormat::Rewarded, node["instance"]);
}

AdFormat RewardedConfig::format() const {
    return AdFormat::Rewarded;
}

std::shared_ptr<IAd> RewardedConfig::createAd(
    const std::shared_ptr<INetworkConfigManager>& manager) const {
    auto ad = instance_->createAd(manager);
    return std::make_shared<GenericAd>(ad, 0);
}

template <class Ad>
template <class MultiAd>
std::shared_ptr<IAdInstanceConfig<Ad>>
IAdInstanceConfig<Ad>::parse(AdFormat format, const nlohmann::json& node) {
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
std::shared_ptr<Ad> SingleInstanceConfig<Ad>::createAd(
    const std::shared_ptr<INetworkConfigManager>& manager) const {
    auto ad = manager->createAd(network_, format_, id_);
    return std::dynamic_pointer_cast<Ad>(ad);
}

template <class Ad, class MultiAd>
WaterfallInstanceConfig<Ad, MultiAd>::WaterfallInstanceConfig(
    AdFormat format, const nlohmann::json& node) {
    for (auto&& [key, value] : node.items()) {
        instances_.push_back(
            IAdInstanceConfig<Ad>::template parse<MultiAd>(format, value));
    }
}

template <class Ad, class MultiAd>
std::shared_ptr<Ad> WaterfallInstanceConfig<Ad, MultiAd>::createAd(
    const std::shared_ptr<INetworkConfigManager>& manager) const {
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
    result->networkManager_ = std::make_shared<NetworkConfigManager>(node);
    result->adManager_ =
        std::make_shared<AdConfigManager>(result->networkManager_, node);
    return result;
}

Task<> Self::initialize() {
    co_await networkManager_->initialize();
}

std::shared_ptr<IAd> Self::createAd(AdFormat format) {
    return adManager_->createAd(format);
}
} // namespace services
} // namespace ee
