using System;
using System.Collections.Generic;
using System.Threading.Tasks;

using SimpleJSON;

using UnityEngine.Assertions;

namespace EE.Internal {
    internal enum Network {
        AdMob,
        FacebookAds,
        IronSource,
        UnityAds
    }

    internal enum AdFormat {
        Banner,
        Rectangle,
        AppOpen,
        Interstitial,
        Rewarded
    }

    internal interface INetworkConfigManager {
        Task Initialize();
        IAd CreateAd(Network network, AdFormat format, string id);
    }

    internal class NetworkConfigManager : INetworkConfigManager {
        private readonly List<INetworkConfig> _networks = new List<INetworkConfig>();

        public NetworkConfigManager(JSONNode node) {
            foreach (var value in node["networks"].Children) {
                var network = NetworkConfig.Parse(value);
                _networks.Add(network);
            }
        }

        public async Task Initialize() {
            foreach (var network in _networks) {
                await network.Initialize();
            }
        }

        public IAd CreateAd(Network network, AdFormat format, string id) {
            foreach (var item in _networks) {
                if (item.Network == network) {
                    return item.CreateAd(format, id);
                }
            }
            return null;
        }
    }

    internal interface INetworkConfig {
        Task Initialize();
        Network Network { get; }
        IAd CreateAd(AdFormat format, string id);
    }

    internal static class NetworkConfig {
        public static INetworkConfig Parse(JSONNode node) {
            var network = node["network"].Value;
            switch (network) {
                case "ad_mob": return new AdMobConfig(node);
                case "facebook_ads": return new FacebookAdsConfig(node);
                case "iron_source": return new IronSourceConfig(node);
                case "unity_ads": return new UnityAdsConfig(node);
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }
    }

    internal class AdMobConfig : INetworkConfig {
        private IAdMob _plugin;

        public AdMobConfig(JSONNode node) {
        }

        public async Task Initialize() {
            _plugin = PluginManager.CreatePlugin<IAdMob>();
            await _plugin.Initialize();
        }

        public Network Network => Network.AdMob;

        public IAd CreateAd(AdFormat format, string id) {
            switch (format) {
                case AdFormat.Banner:
                    return _plugin.CreateBannerAd(id, AdMobBannerAdSize.Normal);
                case AdFormat.Rectangle:
                    return _plugin.CreateBannerAd(id, AdMobBannerAdSize.MediumRectangle);
                case AdFormat.AppOpen:
                    return _plugin.CreateAppOpenAd(id);
                case AdFormat.Interstitial:
                    return _plugin.CreateInterstitialAd(id);
                case AdFormat.Rewarded:
                    return _plugin.CreateRewardedAd(id);
                default:
                    Assert.IsTrue(false);
                    return null;
            }
        }
    }

    internal class FacebookAdsConfig : INetworkConfig {
        private IFacebookAds _plugin;

        public FacebookAdsConfig(JSONNode node) {
        }

        public async Task Initialize() {
            _plugin = PluginManager.CreatePlugin<IFacebookAds>();
            await _plugin.Initialize();
        }

        public Network Network => Network.FacebookAds;

        public IAd CreateAd(AdFormat format, string id) {
            switch (format) {
                case AdFormat.Banner:
                    return _plugin.CreateBannerAd(id, FacebookBannerAdSize.BannerHeight50);
                case AdFormat.Rectangle:
                    return _plugin.CreateBannerAd(id, FacebookBannerAdSize.RectangleHeight250);
                case AdFormat.Interstitial:
                    return _plugin.CreateInterstitialAd(id);
                case AdFormat.Rewarded:
                    return _plugin.CreateRewardedAd(id);
                default:
                    Assert.IsTrue(false);
                    return null;
            }
        }
    }

    internal class IronSourceConfig : INetworkConfig {
        private IIronSource _plugin;
        private readonly string _appId;

        public IronSourceConfig(JSONNode node) {
            _appId = node["app_id"];
        }

        public async Task Initialize() {
            _plugin = PluginManager.CreatePlugin<IIronSource>();
            await _plugin.Initialize(_appId);
        }

        public Network Network => Network.IronSource;

        public IAd CreateAd(AdFormat format, string id) {
            switch (format) {
                case AdFormat.Banner:
                    return _plugin.CreateBannerAd(id, IronSourceBannerAdSize.Banner);
                case AdFormat.Rectangle:
                    return _plugin.CreateBannerAd(id, IronSourceBannerAdSize.Rectangle);
                case AdFormat.Interstitial:
                    return _plugin.CreateInterstitialAd(id);
                case AdFormat.Rewarded:
                    return _plugin.CreateRewardedAd(id);
                default:
                    Assert.IsTrue(false);
                    return null;
            }
        }
    }

    internal class UnityAdsConfig : INetworkConfig {
        private IUnityAds _plugin;
        private readonly string _appId;
        private readonly int _timeOut;

        public UnityAdsConfig(JSONNode node) {
            _appId = node["app_id"];
            _timeOut = node.HasKey("time_out") ? node["time_out"].AsInt : 30;
        }

        public async Task Initialize() {
            _plugin = PluginManager.CreatePlugin<IUnityAds>();
            await Task.WhenAny(Task.Delay(_timeOut * 1000), _plugin.Initialize(_appId, false));
        }

        public Network Network => Network.UnityAds;

        public IAd CreateAd(AdFormat format, string id) {
            switch (format) {
                case AdFormat.Interstitial:
                    return _plugin.CreateInterstitialAd(id);
                case AdFormat.Rewarded:
                    return _plugin.CreateRewardedAd(id);
                default:
                    Assert.IsTrue(false);
                    return null;
            }
        }
    }

    internal interface IAdConfigManager {
        IAd CreateAd(AdFormat format);
    }

    internal class AdConfigManager : IAdConfigManager {
        private readonly INetworkConfigManager _manager;
        private readonly List<IAdConfig> _ads = new List<IAdConfig>();

        public AdConfigManager(INetworkConfigManager manager, JSONNode node) {
            _manager = manager;
            foreach (var value in node["ads"].Children) {
                var ad = AdConfig.Parse(value);
                _ads.Add(ad);
            }
        }

        public IAd CreateAd(AdFormat format) {
            foreach (var ad in _ads) {
                if (ad.Format == format) {
                    return ad.CreateAd(_manager);
                }
            }
            return null;
        }
    }

    internal interface IAdConfig {
        AdFormat Format { get; }
        IAd CreateAd(INetworkConfigManager manager);
    }

    internal static class AdConfig {
        public static IAdConfig Parse(JSONNode node) {
            var format = node["format"].Value;
            switch (format) {
                case "banner": return new BannerConfig(node);
                case "rect": return new RectangleConfig(node);
                case "app_open": return new AppOpenConfig(node);
                case "interstitial": return new InterstitialConfig(node);
                case "rewarded": return new RewardedConfig(node);
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }
    }

    internal class BannerConfig : IAdConfig {
        private readonly IAdInstanceConfig<IBannerAd> _instance;

        public BannerConfig(JSONNode node) {
            _instance = AdInstanceConfig<IBannerAd>.Parse<MultiBannerAd>(AdFormat.Banner, node["instance"]);
        }

        public AdFormat Format => AdFormat.Banner;

        public IAd CreateAd(INetworkConfigManager manager) {
            var ad = _instance.CreateAd(manager);
            return new UnityBannerAd(ad);
        }
    }

    internal class RectangleConfig : IAdConfig {
        private readonly IAdInstanceConfig<IBannerAd> _instance;

        public RectangleConfig(JSONNode node) {
            _instance = AdInstanceConfig<IBannerAd>.Parse<MultiBannerAd>(AdFormat.Rectangle, node["instance"]);
        }

        public AdFormat Format => AdFormat.Rectangle;

        public IAd CreateAd(INetworkConfigManager manager) {
            var ad = _instance.CreateAd(manager);
            return new UnityBannerAd(ad);
        }
    }

    internal class AppOpenConfig : IAdConfig {
        private readonly int _interval;
        private readonly IAdInstanceConfig<IFullScreenAd> _instance;

        public AppOpenConfig(JSONNode node) {
            _interval = node.HasKey("interval") ? node["interval"].AsInt : 0;
            _instance = AdInstanceConfig<IFullScreenAd>.Parse<MultiFullScreenAd>(AdFormat.AppOpen, node["instance"]);
        }

        public AdFormat Format => AdFormat.AppOpen;

        public IAd CreateAd(INetworkConfigManager manager) {
            var ad = _instance.CreateAd(manager);
            return new GenericAd(ad, _interval);
        }
    }

    internal class InterstitialConfig : IAdConfig {
        private readonly int _interval;
        private readonly IAdInstanceConfig<IFullScreenAd> _instance;

        public InterstitialConfig(JSONNode node) {
            _interval = node.HasKey("interval") ? node["interval"].AsInt : 0;
            _instance = AdInstanceConfig<IFullScreenAd>.Parse<MultiFullScreenAd>(AdFormat.Interstitial,
                node["instance"]);
        }

        public AdFormat Format => AdFormat.Interstitial;

        public IAd CreateAd(INetworkConfigManager manager) {
            var ad = _instance.CreateAd(manager);
            return new GenericAd(ad, _interval);
        }
    }

    internal class RewardedConfig : IAdConfig {
        private readonly IAdInstanceConfig<IFullScreenAd> _instance;

        public RewardedConfig(JSONNode node) {
            _instance = AdInstanceConfig<IFullScreenAd>.Parse<MultiFullScreenAd>(AdFormat.Rewarded, node["instance"]);
        }

        public AdFormat Format => AdFormat.Rewarded;

        public IAd CreateAd(INetworkConfigManager manager) {
            var ad = _instance.CreateAd(manager);
            return new GenericAd(ad, 0);
        }
    }

    internal interface IAdInstanceConfig<out Ad> where Ad : IAd {
        Ad CreateAd(INetworkConfigManager manager);
    }

    internal static class AdInstanceConfig<Ad> where Ad : class, IAd {
        public static IAdInstanceConfig<Ad> Parse<MultiAd>(AdFormat format, JSONNode node)
            where MultiAd : IMultiAd<Ad>, Ad, new() {
            if (node.IsArray) {
                return new WaterfallInstanceConfig<Ad, MultiAd>(format, node);
            }
            return new SingleInstanceConfig<Ad>(format, node);
        }
    }

    internal class SingleInstanceConfig<Ad> : IAdInstanceConfig<Ad> where Ad : class, IAd {
        private readonly AdFormat _format;
        private readonly Network _network;
        private readonly string _id;

        public SingleInstanceConfig(AdFormat format, JSONNode node) {
            _format = format;
            var networks = new Dictionary<string, Network> {
                ["ad_mob"] = Network.AdMob,
                ["facebook_ads"] = Network.FacebookAds,
                ["iron_source"] = Network.IronSource,
                ["unity_ads"] = Network.UnityAds
            };
            var network = node["network"].Value;
            _network = networks[network];
            _id = node.HasKey("id") ? node["id"].Value : "";
        }

        public Ad CreateAd(INetworkConfigManager manager) {
            var ad = manager.CreateAd(_network, _format, _id);
            return ad as Ad;
        }
    }

    internal class WaterfallInstanceConfig<Ad, MultiAd> : IAdInstanceConfig<Ad>
        where Ad : class, IAd
        where MultiAd : IMultiAd<Ad>, Ad, new() {
        private readonly List<IAdInstanceConfig<Ad>> _instances = new List<IAdInstanceConfig<Ad>>();

        public WaterfallInstanceConfig(AdFormat format, JSONNode node) {
            foreach (var value in node.Children) {
                _instances.Add(AdInstanceConfig<Ad>.Parse<MultiAd>(format, value));
            }
        }

        public Ad CreateAd(INetworkConfigManager manager) {
            var ad = new MultiAd();
            foreach (var instance in _instances) {
                ad.AddItem(instance.CreateAd(manager));
            }
            return ad;
        }
    }

    internal class AdsConfig {
        private INetworkConfigManager _networkManager;
        private IAdConfigManager _adManager;

        public static AdsConfig Parse(string text) {
            var node = JSON.Parse(text);
            return Parse(node);
        }

        private static AdsConfig Parse(JSONNode node) {
            var networkManager = new NetworkConfigManager(node);
            var adManager = new AdConfigManager(networkManager, node);
            var result = new AdsConfig {
                _networkManager = networkManager,
                _adManager = adManager
            };
            return result;
        }

        public async Task Initialize() {
            await _networkManager.Initialize();
        }

        public IAd CreateAd(AdFormat format) {
            return _adManager.CreateAd(format);
        }
    }
}