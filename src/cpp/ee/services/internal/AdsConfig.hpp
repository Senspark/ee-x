#ifndef EE_X_ADS_CONFIG_HPP
#define EE_X_ADS_CONFIG_HPP

#include "ee/services/ServicesFwd.hpp"

#include <ee/nlohmann/json_fwd.hpp>

namespace ee {
namespace services {
enum class Network {
    AdMob,
    AppLovin,
    FacebookAds,
    IronSource,
    UnityAds,
    Vungle,
};

enum class AdFormat {
    Banner,
    AppOpen,
    Interstitial,
    Rewarded,
};

class INetworkConfigManager;

class NetworkConfig {
public:
    static std::shared_ptr<NetworkConfig> parse(const nlohmann::json& node);

    virtual ~NetworkConfig() = default;

    virtual Task<> initialize() = 0;
    virtual Network network() const = 0;
    virtual std::shared_ptr<IAd> createAd(AdFormat format,
                                          const std::string& id) = 0;
};

class AdMobConfig : public NetworkConfig {
public:
    explicit AdMobConfig(const nlohmann::json& node);

    virtual Task<> initialize() override;
    virtual Network network() const override;
    virtual std::shared_ptr<IAd> createAd(AdFormat format,
                                          const std::string& id) override;

private:
    std::shared_ptr<IAdMob> plugin_;
};

class AppLovinConfig : public NetworkConfig {
public:
    explicit AppLovinConfig(const nlohmann::json& node);

    virtual Task<> initialize() override;
    virtual Network network() const override;
    virtual std::shared_ptr<IAd> createAd(AdFormat format,
                                          const std::string& id) override;

private:
    std::shared_ptr<IAppLovin> plugin_;
    std::string appId_;
};

class FacebookAdsConfig : public NetworkConfig {
public:
    explicit FacebookAdsConfig(const nlohmann::json& node);

    virtual Task<> initialize() override;
    virtual Network network() const override;
    virtual std::shared_ptr<IAd> createAd(AdFormat format,
                                          const std::string& id) override;

private:
    std::shared_ptr<IFacebookAds> plugin_;
};

class IronSourceConfig : public NetworkConfig {
public:
    explicit IronSourceConfig(const nlohmann::json& node);

    virtual Task<> initialize() override;
    virtual Network network() const override;
    virtual std::shared_ptr<IAd> createAd(AdFormat format,
                                          const std::string& id) override;

private:
    std::shared_ptr<IIronSource> plugin_;
    std::string appId_;
};

class UnityAdsConfig : public NetworkConfig {
public:
    explicit UnityAdsConfig(const nlohmann::json& node);

    virtual Task<> initialize() override;
    virtual Network network() const override;
    virtual std::shared_ptr<IAd> createAd(AdFormat format,
                                          const std::string& id) override;

private:
    std::shared_ptr<IUnityAds> plugin_;
    std::string appId_;
};

class VungleConfig : public NetworkConfig {
public:
    explicit VungleConfig(const nlohmann::json& node);

    virtual Task<> initialize() override;
    virtual Network network() const override;
    virtual std::shared_ptr<IAd> createAd(AdFormat format,
                                          const std::string& id) override;

private:
    std::shared_ptr<IVungle> plugin_;
    std::string appId_;
};

class AdConfig {
public:
    static std::shared_ptr<AdConfig> parse(const nlohmann::json& node);

    virtual ~AdConfig() = default;

    virtual AdFormat format() const = 0;

    virtual std::shared_ptr<IAd>
    createAd(INetworkConfigManager& manager) const = 0;
};

class BannerConfig : public AdConfig {
public:
    explicit BannerConfig(const nlohmann::json& node);

    virtual AdFormat format() const override;

    virtual std::shared_ptr<IAd>
    createAd(INetworkConfigManager& manager) const override;

private:
    std::shared_ptr<AdInstanceConfig<IAdView>> instance_;
};

class AppOpenConfig : public AdConfig {
public:
    explicit AppOpenConfig(const nlohmann::json& node);

    virtual AdFormat format() const override;

    virtual std::shared_ptr<IAd>
    createAd(INetworkConfigManager& manager) const override;

    int interval() const;

private:
    int interval_;
    std::shared_ptr<AdInstanceConfig<IFullScreenAd>> instance_;
};

class InterstitialConfig : public AdConfig {
public:
    explicit InterstitialConfig(const nlohmann::json& node);

    virtual AdFormat format() const override;

    virtual std::shared_ptr<IAd>
    createAd(INetworkConfigManager& manager) const override;

    int interval() const;

private:
    int interval_;
    std::shared_ptr<AdInstanceConfig<IFullScreenAd>> instance_;
};

class RewardedConfig : public AdConfig {
public:
    explicit RewardedConfig(const nlohmann::json& node);

    virtual AdFormat format() const override;

    virtual std::shared_ptr<IAd>
    createAd(INetworkConfigManager& manager) const override;

private:
    std::shared_ptr<AdInstanceConfig<IFullScreenAd>> instance_;
};

template <class Ad>
class AdInstanceConfig {
public:
    template <class MultiAd>
    static std::shared_ptr<AdInstanceConfig> parse(AdFormat format,
                                                   const nlohmann::json& node);

    virtual ~AdInstanceConfig() = default;

    virtual std::shared_ptr<Ad>
    createAd(INetworkConfigManager& manager) const = 0;
};

template <class Ad>
class SingleInstanceConfig : public AdInstanceConfig<Ad> {
public:
    explicit SingleInstanceConfig(AdFormat format, const nlohmann::json& node);

    virtual std::shared_ptr<Ad>
    createAd(INetworkConfigManager& manager) const override;

private:
    AdFormat format_;
    Network network_;
    std::string id_;
};

template <class Ad, class MultiAd>
class WaterfallInstanceConfig : public AdInstanceConfig<Ad> {
public:
    explicit WaterfallInstanceConfig(AdFormat format,
                                     const nlohmann::json& node);

    virtual std::shared_ptr<Ad>
    createAd(INetworkConfigManager& manager) const override;

private:
    AdFormat format_;
    std::vector<std::shared_ptr<AdInstanceConfig<Ad>>> instances_;
};

class INetworkConfigManager {
public:
    virtual std::shared_ptr<IAd> createAd(Network network, AdFormat format,
                                          const std::string& id) = 0;
};

class AdsConfig : public INetworkConfigManager {
public:
    static std::shared_ptr<AdsConfig> parse(const std::string& text);

    virtual std::shared_ptr<IAd> createAd(Network network, AdFormat format,
                                          const std::string& id) override;

    Task<> initialize();
    std::shared_ptr<IAd> createAd(AdFormat format);

private:
    static std::shared_ptr<AdsConfig> parse(const nlohmann::json& node);

    std::map<Network, std::shared_ptr<NetworkConfig>> networks_;
    std::vector<std::shared_ptr<AdConfig>> ads_;
};
} // namespace services
} // namespace ee

#endif // EE_X_ADS_CONFIG_HPP
