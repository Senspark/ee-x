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

class NetworkConfig {
public:
    static std::shared_ptr<NetworkConfig> parseImpl(const nlohmann::json& node);

    virtual ~NetworkConfig() = default;

    Network network() const;

protected:
    Network network_;
};

class AdMobConfig : public NetworkConfig {
public:
    explicit AdMobConfig(const nlohmann::json& node);
};

class AppLovinConfig : public NetworkConfig {
public:
    explicit AppLovinConfig(const nlohmann::json& node);

    const std::string& appId() const;

private:
    std::string appId_;
};

class FacebookAdsConfig : public NetworkConfig {
public:
    explicit FacebookAdsConfig(const nlohmann::json& node);
};

class IronSourceConfig : public NetworkConfig {
public:
    explicit IronSourceConfig(const nlohmann::json& node);

    const std::string& appId() const;

private:
    std::string appId_;
};

class UnityAdsConfig : public NetworkConfig {
public:
    explicit UnityAdsConfig(const nlohmann::json& node);

    const std::string& appId() const;

private:
    std::string appId_;
};

class VungleConfig : public NetworkConfig {
public:
    explicit VungleConfig(const nlohmann::json& node);

    const std::string& appId() const;

private:
    std::string appId_;
};

class AdConfig {
public:
    static std::shared_ptr<AdConfig> parseImpl(const nlohmann::json& node);

    virtual ~AdConfig() = default;

    AdFormat adFormat() const;
    const std::shared_ptr<AdInstanceConfig>& instance() const;

protected:
    AdFormat adFormat_;
    std::shared_ptr<AdInstanceConfig> instance_;
};

class BannerConfig : public AdConfig {
public:
    explicit BannerConfig(const nlohmann::json& node);
};

class AppOpenConfig : public AdConfig {
public:
    explicit AppOpenConfig(const nlohmann::json& node);

    int interval() const;

private:
    int interval_;
};

class InterstitialConfig : public AdConfig {
public:
    explicit InterstitialConfig(const nlohmann::json& node);

    int interval() const;

private:
    int interval_;
};

class RewardedConfig : public AdConfig {
public:
    explicit RewardedConfig(const nlohmann::json& node);
};

class AdInstanceConfig {
public:
    static std::shared_ptr<AdInstanceConfig>
    parseImpl(const nlohmann::json& node);

    virtual ~AdInstanceConfig() = default;
};

class SingleInstanceConfig : public AdInstanceConfig {
public:
    explicit SingleInstanceConfig(const nlohmann::json& node);

    Network network() const;
    const std::string& id() const;

private:
    Network network_;
    std::string id_;
};

class WaterfallInstanceConfig : public AdInstanceConfig {
public:
    explicit WaterfallInstanceConfig(const nlohmann::json& node);

    const std::vector<std::shared_ptr<AdInstanceConfig>>& instances() const;

private:
    std::vector<std::shared_ptr<AdInstanceConfig>> instances_;
};

class AdsConfig {
public:
    static std::shared_ptr<AdsConfig> parse(const std::string& text);

    const std::vector<std::shared_ptr<NetworkConfig>>& networks() const;
    const std::vector<std::shared_ptr<AdConfig>>& ads() const;

private:
    static std::shared_ptr<AdsConfig> parse(const nlohmann::json& node);

    std::vector<std::shared_ptr<NetworkConfig>> networks_;
    std::vector<std::shared_ptr<AdConfig>> ads_;
};
} // namespace services
} // namespace ee

#endif // EE_X_ADS_CONFIG_HPP