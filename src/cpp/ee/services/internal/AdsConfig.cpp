#include "ee/services/internal/AdsConfig.hpp"

#include <cassert>

#include <ee/ad_mob/AdMobBannerAdSize.hpp>
#include <ee/ad_mob/IAdMobBridge.hpp>
#include <ee/ads/AdFormat.hpp>
#include <ee/ads/AdNetwork.hpp>
#include <ee/ads/MultiBannerAd.hpp>
#include <ee/ads/MultiFullScreenAd.hpp>
#include <ee/ads/NullAd.hpp>
#include <ee/ads/NullBannerAd.hpp>
#include <ee/ads/NullFullScreenAd.hpp>
#include <ee/ads/internal/Capper.hpp>
#include <ee/ads/internal/LockCapper.hpp>
#include <ee/ads/internal/NullRetrier.hpp>
#include <ee/ads/internal/Retrier.hpp>
#include <ee/app_lovin/IAppLovinBridge.hpp>
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
#include <ee/unity_ads/UnityBannerAdSize.hpp>
#include <ee/vungle/IVungleBridge.hpp>

#include "ee/services/internal/GenericBannerAd.hpp"
#include "ee/services/internal/GenericFullScreenAd.hpp"

namespace ee {
namespace services {
AdNetwork AdsConfigUtils::parseNetwork(const std::string& id) {
    if (id == "ad_mob") {
        return AdNetwork::AdMob;
    }
    if (id == "app_lovin") {
        return AdNetwork::AppLovin;
    }
    if (id == "facebook_ads") {
        return AdNetwork::FacebookAds;
    }
    if (id == "iron_source") {
        return AdNetwork::IronSource;
    }
    if (id == "unity_ads") {
        return AdNetwork::UnityAds;
    }
    if (id == "vungle") {
        return AdNetwork::Vungle;
    }
    return AdNetwork::Null;
}

AdFormat AdsConfigUtils::parseFormat(const std::string& id) {
    if (id == "banner") {
        return AdFormat::Banner;
    }
    if (id == "rect") {
        return AdFormat::Rectangle;
    }
    if (id == "app_open") {
        return AdFormat::AppOpen;
    }
    if (id == "interstitial") {
        return AdFormat::Interstitial;
    }
    if (id == "rewarded_interstitial") {
        return AdFormat::RewardedInterstitial;
    }
    if (id == "rewarded") {
        return AdFormat::Rewarded;
    }
    return AdFormat::Null;
}

IntervalConfig::IntervalConfig(const nlohmann::json& node,
                               const std::string& key) {
    auto iter = node.find(key);
    if (iter == node.end()) {
        value_ = std::nullopt;
    } else {
        value_ = *iter;
    }
}

std::shared_ptr<ads::ICapper> IntervalConfig::create() const {
    if (value_.has_value()) {
        return std::make_shared<ads::Capper>(value_.value());
    }
    return std::make_shared<ads::LockCapper>();
}

RetrierConfig::RetrierConfig(const nlohmann::json& node,
                             const std::string& key) {
    auto iter = node.find(key);
    if (iter == node.end()) {
        value_ = std::nullopt;
    } else {
        value_ = *iter;
    }
}

std::shared_ptr<ads::IRetrier> RetrierConfig::create() const {
    if (value_.has_value()) {
        return std::make_shared<ads::Retrier>(
            value_.value()[0], value_.value()[1], value_.value()[2]);
    }
    return std::make_shared<ads::NullRetrier>();
}

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

void NetworkConfigManager::addTestDevice(const std::string& hash) {
    for (auto&& item : networks_) {
        item->addTestDevice(hash);
    }
}

void NetworkConfigManager::openTestSuite(AdNetwork network) {
    for (auto&& item : networks_) {
        if (item->network() == network) {
            item->openTestSuite();
            break;
        }
    }
}

std::shared_ptr<IAd> NetworkConfigManager::createAd(AdNetwork network,
                                                    AdFormat format,
                                                    const std::string& id) {
    for (auto&& item : networks_) {
        if (item->network() == network) {
            return item->createAd(format, id);
        }
    }
    return std::make_shared<NullAd>();
}

std::shared_ptr<INetworkConfig>
INetworkConfig::parse(const nlohmann::json& node) {
    auto&& network = AdsConfigUtils::parseNetwork(node["network"]);
    switch (network) {
    case AdNetwork::AdMob:
        return std::make_shared<AdMobConfig>(node);
    case AdNetwork::AppLovin:
        return std::make_shared<AppLovinConfig>(node);
    case AdNetwork::FacebookAds:
        return std::make_shared<FacebookAdsConfig>(node);
    case AdNetwork::IronSource:
        return std::make_shared<IronSourceConfig>(node);
    case AdNetwork::UnityAds:
        return std::make_shared<UnityAdsConfig>(node);
    case AdNetwork::Vungle:
        return std::make_shared<VungleConfig>(node);
    case AdNetwork::Null:
        return std::make_shared<NullNetworkConfig>();
    }
}

AdMobConfig::AdMobConfig(const nlohmann::json& node) {}

Task<> AdMobConfig::initialize() {
    plugin_ = PluginManager::createPlugin<IAdMob>();
    co_await plugin_->initialize();
}

AdNetwork AdMobConfig::network() const {
    return AdNetwork::AdMob;
}

void AdMobConfig::addTestDevice(const std::string& hash) {
    plugin_->addTestDevice(hash);
}

void AdMobConfig::openTestSuite() {
    plugin_->openTestSuite();
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
    case AdFormat::RewardedInterstitial:
        return plugin_->createRewardedInterstitialAd(id);
    case AdFormat::Rewarded:
        return plugin_->createRewardedAd(id);
    case AdFormat::Null:
        return std::make_shared<NullAd>();
    }
}

AppLovinConfig::AppLovinConfig(const nlohmann::json& node) {
    appId_ = node["app_id"];
}

Task<> AppLovinConfig::initialize() {
    plugin_ = PluginManager::createPlugin<IAppLovin>();
    co_await plugin_->initialize(appId_);
}

AdNetwork AppLovinConfig::network() const {
    return AdNetwork::AppLovin;
}

void AppLovinConfig::addTestDevice(const std::string& hash) {}

void AppLovinConfig::openTestSuite() {}

std::shared_ptr<IAd> AppLovinConfig::createAd(AdFormat format,
                                              const std::string& id) {
    switch (format) {
    case AdFormat::Banner:
    case AdFormat::Rectangle:
        return std::make_shared<NullBannerAd>();
    case AdFormat::AppOpen:
    case AdFormat::Interstitial:
    case AdFormat::RewardedInterstitial:
        return std::make_shared<NullFullScreenAd>();
    case AdFormat::Rewarded:
        return plugin_->createRewardedAd();
    case AdFormat::Null:
        return std::make_shared<NullAd>();
    }
}

FacebookAdsConfig::FacebookAdsConfig(const nlohmann::json& node) {}

Task<> FacebookAdsConfig::initialize() {
    plugin_ = PluginManager::createPlugin<IFacebookAds>();
    co_await plugin_->initialize();
}

AdNetwork FacebookAdsConfig::network() const {
    return AdNetwork::FacebookAds;
}

void FacebookAdsConfig::addTestDevice(const std::string& hash) {
    plugin_->addTestDevice(hash);
}

void FacebookAdsConfig::openTestSuite() {}

std::shared_ptr<IAd> FacebookAdsConfig::createAd(AdFormat format,
                                                 const std::string& id) {
    switch (format) {
    case AdFormat::Banner:
        return plugin_->createBannerAd(id,
                                       FacebookBannerAdSize::BannerHeight50);
    case AdFormat::Rectangle:
        return plugin_->createBannerAd(
            id, FacebookBannerAdSize::RectangleHeight250);
    case AdFormat::AppOpen:
    case AdFormat::RewardedInterstitial:
        return std::make_shared<NullFullScreenAd>();
    case AdFormat::Interstitial:
        return plugin_->createInterstitialAd(id);
    case AdFormat::Rewarded:
        return plugin_->createRewardedAd(id);
    case AdFormat::Null:
        return std::make_shared<NullAd>();
    }
}

IronSourceConfig::IronSourceConfig(const nlohmann::json& node) {
    appId_ = node["app_id"];
}

Task<> IronSourceConfig::initialize() {
    plugin_ = PluginManager::createPlugin<IIronSource>();
    co_await plugin_->initialize(appId_);
}

AdNetwork IronSourceConfig::network() const {
    return AdNetwork::IronSource;
}

void IronSourceConfig::addTestDevice(const std::string& hash) {}

void IronSourceConfig::openTestSuite() {}

std::shared_ptr<IAd> IronSourceConfig::createAd(AdFormat format,
                                                const std::string& id) {
    switch (format) {
    case AdFormat::Banner:
        return plugin_->createBannerAd(id, IronSourceBannerAdSize::Banner);
    case AdFormat::Rectangle:
        return plugin_->createBannerAd(id, IronSourceBannerAdSize::Rectangle);
    case AdFormat::AppOpen:
    case AdFormat::RewardedInterstitial:
        return std::make_shared<NullFullScreenAd>();
    case AdFormat::Interstitial:
        return plugin_->createInterstitialAd(id);
    case AdFormat::Rewarded:
        return plugin_->createRewardedAd(id);
    case AdFormat::Null:
        return std::make_shared<NullAd>();
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

AdNetwork UnityAdsConfig::network() const {
    return AdNetwork::UnityAds;
}

void UnityAdsConfig::addTestDevice(const std::string& hash) {}

void UnityAdsConfig::openTestSuite() {}

std::shared_ptr<IAd> UnityAdsConfig::createAd(AdFormat format,
                                              const std::string& id) {
    switch (format) {
    case AdFormat::Banner:
        return plugin_->createBannerAd(id, UnityBannerAdSize::Banner);
    case AdFormat::Rectangle:
        return std::make_shared<NullBannerAd>();
    case AdFormat::AppOpen:
    case AdFormat::RewardedInterstitial:
        return std::make_shared<NullFullScreenAd>();
    case AdFormat::Interstitial:
        return plugin_->createInterstitialAd(id);
    case AdFormat::Rewarded:
        return plugin_->createRewardedAd(id);
    case AdFormat::Null:
        return std::make_shared<NullAd>();
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

AdNetwork VungleConfig::network() const {
    return AdNetwork::Vungle;
}

void VungleConfig::addTestDevice(const std::string& hash) {}

void VungleConfig::openTestSuite() {}

std::shared_ptr<IAd> VungleConfig::createAd(AdFormat format,
                                            const std::string& id) {
    switch (format) {
    case AdFormat::Banner:
    case AdFormat::Rectangle:
        return std::make_shared<NullBannerAd>();
    case AdFormat::AppOpen:
    case AdFormat::Interstitial:
    case AdFormat::RewardedInterstitial:
        return std::make_shared<NullFullScreenAd>();
    case AdFormat::Rewarded:
        return plugin_->createRewardedAd(id);
    case AdFormat::Null:
        return std::make_shared<NullAd>();
    }
}

Task<> NullNetworkConfig::initialize() {
    co_return;
}

AdNetwork NullNetworkConfig::network() const {
    return AdNetwork::Null;
}

void NullNetworkConfig::addTestDevice(const std::string& hash) {}

void NullNetworkConfig::openTestSuite() {}

std::shared_ptr<IAd> NullNetworkConfig::createAd(AdFormat format,
                                                 const std::string& id) {
    switch (format) {
    case AdFormat::Banner:
    case AdFormat::Rectangle:
        return std::make_shared<NullBannerAd>();
    case AdFormat::AppOpen:
    case AdFormat::Interstitial:
    case AdFormat::RewardedInterstitial:
    case AdFormat::Rewarded:
        return std::make_shared<NullFullScreenAd>();
    case AdFormat::Null:
        return std::make_shared<NullAd>();
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
    return std::make_shared<NullAd>();
}

std::shared_ptr<IAdConfig> IAdConfig::parse(const nlohmann::json& node) {
    auto&& format = AdsConfigUtils::parseFormat(node["format"]);
    switch (format) {
    case AdFormat::Banner:
        return std::make_shared<BannerConfig>(node);
    case AdFormat::Rectangle:
        return std::make_shared<RectangleConfig>(node);
    case AdFormat::AppOpen:
        return std::make_shared<AppOpenConfig>(node);
    case AdFormat::Interstitial:
        return std::make_shared<InterstitialConfig>(node);
    case AdFormat::RewardedInterstitial:
        return std::make_shared<RewardedInterstitialConfig>(node);
    case AdFormat::Rewarded:
        return std::make_shared<RewardedConfig>(node);
    case AdFormat::Null:
        return std::make_shared<NullAdConfig>();
    }
}

BannerConfig::BannerConfig(const nlohmann::json& node) {
    loadConfig_ = std::make_shared<IntervalConfig>(node, "load_interval");
    retrierConfig_ = std::make_shared<RetrierConfig>(node, "reload_params");
    instance_ = IAdInstanceConfig<IBannerAd>::parse<MultiBannerAd>(
        AdFormat::Banner, node["instance"]);
}

AdFormat BannerConfig::format() const {
    return AdFormat::Banner;
}

std::shared_ptr<IAd> BannerConfig::createAd(
    const std::shared_ptr<INetworkConfigManager>& manager) const {
    auto ad = instance_->createAd(manager);
    return std::make_shared<GenericBannerAd>(ad, loadConfig_->create(),
                                             retrierConfig_->create());
}

RectangleConfig::RectangleConfig(const nlohmann::json& node) {
    loadConfig_ = std::make_shared<IntervalConfig>(node, "load_interval");
    retrierConfig_ = std::make_shared<RetrierConfig>(node, "reload_params");
    instance_ = IAdInstanceConfig<IBannerAd>::parse<MultiBannerAd>(
        AdFormat::Rectangle, node["instance"]);
}

AdFormat RectangleConfig::format() const {
    return AdFormat::Rectangle;
}

std::shared_ptr<IAd> RectangleConfig::createAd(
    const std::shared_ptr<INetworkConfigManager>& manager) const {
    auto ad = instance_->createAd(manager);
    return std::make_shared<GenericBannerAd>(ad, loadConfig_->create(),
                                             retrierConfig_->create());
}

AppOpenConfig::AppOpenConfig(const nlohmann::json& node) {
    displayConfig_ = std::make_shared<IntervalConfig>(node, "interval");
    loadConfig_ = std::make_shared<IntervalConfig>(node, "load_interval");
    retrierConfig_ = std::make_shared<RetrierConfig>(node, "reload_params");
    instance_ = IAdInstanceConfig<IFullScreenAd>::parse<MultiFullScreenAd>(
        AdFormat::AppOpen, node["instance"]);
}

AdFormat AppOpenConfig::format() const {
    return AdFormat::AppOpen;
}

std::shared_ptr<IAd> AppOpenConfig::createAd(
    const std::shared_ptr<INetworkConfigManager>& manager) const {
    auto ad = instance_->createAd(manager);
    return std::make_shared<GenericFullScreenAd>(ad, displayConfig_->create(),
                                                 loadConfig_->create(),
                                                 retrierConfig_->create());
}

InterstitialConfig::InterstitialConfig(const nlohmann::json& node) {
    displayConfig_ = std::make_shared<IntervalConfig>(node, "interval");
    loadConfig_ = std::make_shared<IntervalConfig>(node, "load_interval");
    retrierConfig_ = std::make_shared<RetrierConfig>(node, "reload_params");
    instance_ = IAdInstanceConfig<IFullScreenAd>::parse<MultiFullScreenAd>(
        AdFormat::Interstitial, node["instance"]);
}

AdFormat InterstitialConfig::format() const {
    return AdFormat::Interstitial;
}

std::shared_ptr<IAd> InterstitialConfig::createAd(
    const std::shared_ptr<INetworkConfigManager>& manager) const {
    auto ad = instance_->createAd(manager);
    return std::make_shared<GenericFullScreenAd>(ad, displayConfig_->create(),
                                                 loadConfig_->create(),
                                                 retrierConfig_->create());
}

RewardedInterstitialConfig::RewardedInterstitialConfig(
    const nlohmann::json& node) {
    loadConfig_ = std::make_shared<IntervalConfig>(node, "load_interval");
    retrierConfig_ = std::make_shared<RetrierConfig>(node, "reload_params");
    instance_ = IAdInstanceConfig<IFullScreenAd>::parse<MultiFullScreenAd>(
        AdFormat::RewardedInterstitial, node["instance"]);
}

AdFormat RewardedInterstitialConfig::format() const {
    return AdFormat::RewardedInterstitial;
}

std::shared_ptr<IAd> RewardedInterstitialConfig::createAd(
    const std::shared_ptr<INetworkConfigManager>& manager) const {
    auto ad = instance_->createAd(manager);
    return std::make_shared<GenericFullScreenAd>(
        ad, std::make_shared<ads::LockCapper>(), loadConfig_->create(),
        retrierConfig_->create());
}

RewardedConfig::RewardedConfig(const nlohmann::json& node) {
    loadConfig_ = std::make_shared<IntervalConfig>(node, "load_interval");
    retrierConfig_ = std::make_shared<RetrierConfig>(node, "reload_params");
    instance_ = IAdInstanceConfig<IFullScreenAd>::parse<MultiFullScreenAd>(
        AdFormat::Rewarded, node["instance"]);
}

AdFormat RewardedConfig::format() const {
    return AdFormat::Rewarded;
}

std::shared_ptr<IAd> RewardedConfig::createAd(
    const std::shared_ptr<INetworkConfigManager>& manager) const {
    auto ad = instance_->createAd(manager);
    return std::make_shared<GenericFullScreenAd>(
        ad, std::make_shared<ads::LockCapper>(), loadConfig_->create(),
        retrierConfig_->create());
}

AdFormat NullAdConfig::format() const {
    return AdFormat::Null;
}

std::shared_ptr<IAd> NullAdConfig::createAd(
    const std::shared_ptr<INetworkConfigManager>& manager) const {
    return std::make_shared<NullAd>();
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
    network_ = AdsConfigUtils::parseNetwork(node["network"]);
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

void Self::addTestDevice(const std::string& hash) {
    networkManager_->addTestDevice(hash);
}

void Self::openTestSuite(AdNetwork network) {
    networkManager_->openTestSuite(network);
}

std::shared_ptr<IAd> Self::createAd(AdFormat format) {
    return adManager_->createAd(format);
}
} // namespace services
} // namespace ee
