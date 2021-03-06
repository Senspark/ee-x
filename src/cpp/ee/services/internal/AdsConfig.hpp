#ifndef EE_X_ADS_CONFIG_HPP
#define EE_X_ADS_CONFIG_HPP

#include <ee/ad_mob/AdMobFwd.hpp>
#include <ee/app_lovin/AppLovinFwd.hpp>
#include <ee/facebook_ads/FacebookAdsFwd.hpp>
#include <ee/iron_source/IronSourceFwd.hpp>
#include <ee/nlohmann/json_fwd.hpp>
#include <ee/unity_ads/UnityAdsFwd.hpp>
#include <ee/vungle/VungleFwd.hpp>

#include "ee/services/ServicesFwd.hpp"

namespace ee {
namespace services {
class AdsConfigUtils {
public:
    static AdNetwork parseNetwork(const std::string& id);
    static AdFormat parseFormat(const std::string& id);
};

class IntervalConfig {
public:
    explicit IntervalConfig(const nlohmann::json& node, const std::string& key);
    std::shared_ptr<ads::ICapper> create() const;

private:
    std::optional<int> value_;
};

class RetrierConfig {
public:
    explicit RetrierConfig(const nlohmann::json& node, const std::string& key);
    std::shared_ptr<ads::IRetrier> create() const;

private:
    std::optional<std::vector<int>> value_;
};

class INetworkConfig;

class INetworkConfigManager {
public:
    virtual ~INetworkConfigManager() = default;
    virtual Task<> initialize() = 0;
    virtual void addTestDevice(const std::string& hash) = 0;
    virtual void openTestSuite(AdNetwork network) = 0;
    virtual std::shared_ptr<IAd> createAd(AdNetwork network, AdFormat format,
                                          const std::string& id) = 0;
};

class NetworkConfigManager : public INetworkConfigManager {
public:
    explicit NetworkConfigManager(const nlohmann::json& node);

    virtual Task<> initialize() override;
    virtual void addTestDevice(const std::string& hash) override;
    virtual void openTestSuite(AdNetwork network) override;
    virtual std::shared_ptr<IAd> createAd(AdNetwork network, AdFormat format,
                                          const std::string& id) override;

private:
    std::vector<std::shared_ptr<INetworkConfig>> networks_;
};

class INetworkConfig {
public:
    static std::shared_ptr<INetworkConfig> parse(const nlohmann::json& node);

    virtual ~INetworkConfig() = default;

    virtual Task<> initialize() = 0;
    virtual AdNetwork network() const = 0;
    virtual void addTestDevice(const std::string& hash) = 0;
    virtual void openTestSuite() = 0;
    virtual std::shared_ptr<IAd> createAd(AdFormat format,
                                          const std::string& id) = 0;
};

class AdMobConfig : public INetworkConfig {
public:
    explicit AdMobConfig(const nlohmann::json& node);

    virtual Task<> initialize() override;
    virtual AdNetwork network() const override;
    virtual void addTestDevice(const std::string& hash) override;
    virtual void openTestSuite() override;
    virtual std::shared_ptr<IAd> createAd(AdFormat format,
                                          const std::string& id) override;

private:
    std::shared_ptr<IAdMob> plugin_;
};

class AppLovinConfig : public INetworkConfig {
public:
    explicit AppLovinConfig(const nlohmann::json& node);

    virtual Task<> initialize() override;
    virtual AdNetwork network() const override;
    virtual void addTestDevice(const std::string& hash) override;
    virtual void openTestSuite() override;
    virtual std::shared_ptr<IAd> createAd(AdFormat format,
                                          const std::string& id) override;

private:
    std::shared_ptr<IAppLovin> plugin_;
    std::string appId_;
};

class FacebookAdsConfig : public INetworkConfig {
public:
    explicit FacebookAdsConfig(const nlohmann::json& node);

    virtual Task<> initialize() override;
    virtual AdNetwork network() const override;
    virtual void addTestDevice(const std::string& hash) override;
    virtual void openTestSuite() override;
    virtual std::shared_ptr<IAd> createAd(AdFormat format,
                                          const std::string& id) override;

private:
    std::shared_ptr<IFacebookAds> plugin_;
};

class IronSourceConfig : public INetworkConfig {
public:
    explicit IronSourceConfig(const nlohmann::json& node);

    virtual Task<> initialize() override;
    virtual AdNetwork network() const override;
    virtual void addTestDevice(const std::string& hash) override;
    virtual void openTestSuite() override;
    virtual std::shared_ptr<IAd> createAd(AdFormat format,
                                          const std::string& id) override;

private:
    std::shared_ptr<IIronSource> plugin_;
    std::string appId_;
};

class UnityAdsConfig : public INetworkConfig {
public:
    explicit UnityAdsConfig(const nlohmann::json& node);

    virtual Task<> initialize() override;
    virtual AdNetwork network() const override;
    virtual void addTestDevice(const std::string& hash) override;
    virtual void openTestSuite() override;
    virtual std::shared_ptr<IAd> createAd(AdFormat format,
                                          const std::string& id) override;

private:
    std::shared_ptr<IUnityAds> plugin_;
    std::string appId_;
    int timeOut_;
};

class VungleConfig : public INetworkConfig {
public:
    explicit VungleConfig(const nlohmann::json& node);

    virtual Task<> initialize() override;
    virtual AdNetwork network() const override;
    virtual void addTestDevice(const std::string& hash) override;
    virtual void openTestSuite() override;
    virtual std::shared_ptr<IAd> createAd(AdFormat format,
                                          const std::string& id) override;

private:
    std::shared_ptr<IVungle> plugin_;
    std::string appId_;
    int timeOut_;
};

class NullNetworkConfig : public INetworkConfig {
public:
    virtual Task<> initialize() override;
    virtual AdNetwork network() const override;
    virtual void addTestDevice(const std::string& hash) override;
    virtual void openTestSuite() override;
    virtual std::shared_ptr<IAd> createAd(AdFormat format,
                                          const std::string& id) override;
};

class IAdConfig;

class IAdConfigManager {
public:
    virtual ~IAdConfigManager() = default;
    virtual std::shared_ptr<IAd> createAd(AdFormat format) = 0;
};

class AdConfigManager : public IAdConfigManager {
public:
    explicit AdConfigManager(
        const std::shared_ptr<INetworkConfigManager>& manager,
        const nlohmann::json& node);
    virtual std::shared_ptr<IAd> createAd(AdFormat format) override;

private:
    std::shared_ptr<INetworkConfigManager> manager_;
    std::vector<std::shared_ptr<IAdConfig>> ads_;
};

class IAdConfig {
public:
    static std::shared_ptr<IAdConfig> parse(const nlohmann::json& node);

    virtual ~IAdConfig() = default;

    virtual AdFormat format() const = 0;

    virtual std::shared_ptr<IAd>
    createAd(const std::shared_ptr<INetworkConfigManager>& manager) const = 0;
};

template <class Ad>
class IAdInstanceConfig;

class BannerConfig : public IAdConfig {
public:
    explicit BannerConfig(const nlohmann::json& node);

    virtual AdFormat format() const override;

    virtual std::shared_ptr<IAd> createAd(
        const std::shared_ptr<INetworkConfigManager>& manager) const override;

private:
    std::shared_ptr<IntervalConfig> loadConfig_;
    std::shared_ptr<RetrierConfig> retrierConfig_;
    std::shared_ptr<IAdInstanceConfig<IBannerAd>> instance_;
};

class RectangleConfig : public IAdConfig {
public:
    explicit RectangleConfig(const nlohmann::json& node);

    virtual AdFormat format() const override;

    virtual std::shared_ptr<IAd> createAd(
        const std::shared_ptr<INetworkConfigManager>& manager) const override;

private:
    std::shared_ptr<IntervalConfig> loadConfig_;
    std::shared_ptr<RetrierConfig> retrierConfig_;
    std::shared_ptr<IAdInstanceConfig<IBannerAd>> instance_;
};

class AppOpenConfig : public IAdConfig {
public:
    explicit AppOpenConfig(const nlohmann::json& node);

    virtual AdFormat format() const override;

    virtual std::shared_ptr<IAd> createAd(
        const std::shared_ptr<INetworkConfigManager>& manager) const override;

private:
    std::shared_ptr<IntervalConfig> displayConfig_;
    std::shared_ptr<IntervalConfig> loadConfig_;
    std::shared_ptr<RetrierConfig> retrierConfig_;
    std::shared_ptr<IAdInstanceConfig<IFullScreenAd>> instance_;
};

class InterstitialConfig : public IAdConfig {
public:
    explicit InterstitialConfig(const nlohmann::json& node);

    virtual AdFormat format() const override;

    virtual std::shared_ptr<IAd> createAd(
        const std::shared_ptr<INetworkConfigManager>& manager) const override;

private:
    std::shared_ptr<IntervalConfig> displayConfig_;
    std::shared_ptr<IntervalConfig> loadConfig_;
    std::shared_ptr<RetrierConfig> retrierConfig_;
    std::shared_ptr<IAdInstanceConfig<IFullScreenAd>> instance_;
};

class RewardedInterstitialConfig : public IAdConfig {
public:
    explicit RewardedInterstitialConfig(const nlohmann::json& node);

    virtual AdFormat format() const override;

    virtual std::shared_ptr<IAd> createAd(
        const std::shared_ptr<INetworkConfigManager>& manager) const override;

private:
    std::shared_ptr<IntervalConfig> loadConfig_;
    std::shared_ptr<RetrierConfig> retrierConfig_;
    std::shared_ptr<IAdInstanceConfig<IFullScreenAd>> instance_;
};

class RewardedConfig : public IAdConfig {
public:
    explicit RewardedConfig(const nlohmann::json& node);

    virtual AdFormat format() const override;

    virtual std::shared_ptr<IAd> createAd(
        const std::shared_ptr<INetworkConfigManager>& manager) const override;

private:
    std::shared_ptr<IntervalConfig> loadConfig_;
    std::shared_ptr<RetrierConfig> retrierConfig_;
    std::shared_ptr<IAdInstanceConfig<IFullScreenAd>> instance_;
};

class NullAdConfig : public IAdConfig {
public:
    virtual AdFormat format() const override;

    virtual std::shared_ptr<IAd> createAd(
        const std::shared_ptr<INetworkConfigManager>& manager) const override;
};

template <class Ad>
class IAdInstanceConfig {
public:
    template <class MultiAd>
    static std::shared_ptr<IAdInstanceConfig> parse(AdFormat format,
                                                    const nlohmann::json& node);

    virtual ~IAdInstanceConfig() = default;

    virtual std::shared_ptr<Ad>
    createAd(const std::shared_ptr<INetworkConfigManager>& manager) const = 0;
};

template <class Ad>
class SingleInstanceConfig : public IAdInstanceConfig<Ad> {
public:
    explicit SingleInstanceConfig(AdFormat format, const nlohmann::json& node);

    virtual std::shared_ptr<Ad> createAd(
        const std::shared_ptr<INetworkConfigManager>& manager) const override;

private:
    AdFormat format_;
    AdNetwork network_;
    std::string id_;
};

template <class Ad, class MultiAd>
class WaterfallInstanceConfig : public IAdInstanceConfig<Ad> {
public:
    explicit WaterfallInstanceConfig(AdFormat format,
                                     const nlohmann::json& node);

    virtual std::shared_ptr<Ad> createAd(
        const std::shared_ptr<INetworkConfigManager>& manager) const override;

private:
    std::vector<std::shared_ptr<IAdInstanceConfig<Ad>>> instances_;
};

class AdsConfig {
public:
    static std::shared_ptr<AdsConfig> parse(const std::string& text);

    Task<> initialize();
    void addTestDevice(const std::string& hash);
    void openTestSuite(AdNetwork network);
    std::shared_ptr<IAd> createAd(AdFormat format);

private:
    static std::shared_ptr<AdsConfig> parse(const nlohmann::json& node);

    std::shared_ptr<INetworkConfigManager> networkManager_;
    std::shared_ptr<IAdConfigManager> adManager_;
};
} // namespace services
} // namespace ee

#endif // EE_X_ADS_CONFIG_HPP
