using System;
using System.Collections.Generic;
using System.Threading.Tasks;

using SimpleJSON;

namespace EE.Internal {
    internal static class AdsConfigUtils {
        public static Network ParseNetwork(string id) {
            switch (id) {
                case "ad_mob": return Network.AdMob;
                case "facebook_ads": return Network.FacebookAds;
                case "iron_source": return Network.IronSource;
                case "unity_ads": return Network.UnityAds;
                default: return Network.Null;
            }
        }

        public static AdFormat ParseAdFormat(string id) {
            switch (id) {
                case "banner": return AdFormat.Banner;
                case "rect": return AdFormat.Rectangle;
                case "app_open": return AdFormat.AppOpen;
                case "interstitial": return AdFormat.Interstitial;
                case "rewarded_interstitial": return AdFormat.RewardedInterstitial;
                case "rewarded": return AdFormat.Rewarded;
                default: return AdFormat.Null;
            }
        }
    }

    internal enum Network {
        AdMob,
        FacebookAds,
        IronSource,
        UnityAds,
        Null
    }

    internal enum AdFormat {
        Banner,
        Rectangle,
        AppOpen,
        Interstitial,
        RewardedInterstitial,
        Rewarded,
        Null
    }

    internal interface INetworkConfigManager {
        Task Initialize();
        void OpenTestSuite(Network network);
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

        public void OpenTestSuite(Network network) {
            foreach (var item in _networks) {
                if (item.Network == network) {
                    item.OpenTestSuite();
                    break;
                }
            }
        }

        public IAd CreateAd(Network network, AdFormat format, string id) {
            foreach (var item in _networks) {
                if (item.Network == network) {
                    return item.CreateAd(format, id);
                }
            }
            return new NullAd();
        }
    }

    internal interface INetworkConfig {
        Task Initialize();
        Network Network { get; }
        void OpenTestSuite();
        IAd CreateAd(AdFormat format, string id);
    }

    internal static class NetworkConfig {
        public static INetworkConfig Parse(JSONNode node) {
            var network = AdsConfigUtils.ParseNetwork(node["network"].Value);
            switch (network) {
                case Network.AdMob: return new AdMobConfig(node);
                case Network.FacebookAds: return new FacebookAdsConfig(node);
                case Network.IronSource: return new IronSourceConfig(node);
                case Network.UnityAds: return new UnityAdsConfig(node);
                case Network.Null: return new NullNetworkConfig();
            }
            throw new ArgumentException();
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

        public void OpenTestSuite() {
            _plugin.OpenTestSuite();
        }

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
                case AdFormat.RewardedInterstitial:
                    return _plugin.CreateRewardedInterstitialAd(id);
                case AdFormat.Rewarded:
                    return _plugin.CreateRewardedAd(id);
                case AdFormat.Null:
                    return new NullAd();
            }
            throw new ArgumentException();
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

        public void OpenTestSuite() {
        }

        public IAd CreateAd(AdFormat format, string id) {
            switch (format) {
                case AdFormat.Banner:
                    return _plugin.CreateBannerAd(id, FacebookBannerAdSize.BannerHeight50);
                case AdFormat.Rectangle:
                    return _plugin.CreateBannerAd(id, FacebookBannerAdSize.RectangleHeight250);
                case AdFormat.AppOpen:
                case AdFormat.RewardedInterstitial:
                    return new NullFullScreenAd();
                case AdFormat.Interstitial:
                    return _plugin.CreateInterstitialAd(id);
                case AdFormat.Rewarded:
                    return _plugin.CreateRewardedAd(id);
                case AdFormat.Null:
                    return new NullAd();
            }
            throw new ArgumentException();
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

        public void OpenTestSuite() {
        }

        public IAd CreateAd(AdFormat format, string id) {
            switch (format) {
                case AdFormat.Banner:
                    return _plugin.CreateBannerAd(id, IronSourceBannerAdSize.Banner);
                case AdFormat.Rectangle:
                    return _plugin.CreateBannerAd(id, IronSourceBannerAdSize.Rectangle);
                case AdFormat.AppOpen:
                case AdFormat.RewardedInterstitial:
                    return new NullFullScreenAd();
                case AdFormat.Interstitial:
                    return _plugin.CreateInterstitialAd(id);
                case AdFormat.Rewarded:
                    return _plugin.CreateRewardedAd(id);
                case AdFormat.Null:
                    return new NullAd();
            }
            throw new ArgumentException();
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

        public void OpenTestSuite() {
        }

        public IAd CreateAd(AdFormat format, string id) {
            switch (format) {
                case AdFormat.Banner:
                    return _plugin.CreateBannerAd(id, UnityBannerAdSize.Normal);
                case AdFormat.Rectangle:
                    return new NullBannerAd();
                case AdFormat.AppOpen:
                case AdFormat.RewardedInterstitial:
                    return new NullFullScreenAd();
                case AdFormat.Interstitial:
                    return _plugin.CreateInterstitialAd(id);
                case AdFormat.Rewarded:
                    return _plugin.CreateRewardedAd(id);
                case AdFormat.Null:
                    return new NullAd();
            }
            throw new ArgumentException();
        }
    }

    internal class NullNetworkConfig : INetworkConfig {
        public Task Initialize() {
            return Task.FromResult<object>(null);
        }

        public Network Network => Network.Null;

        public void OpenTestSuite() {
        }

        public IAd CreateAd(AdFormat format, string id) {
            switch (format) {
                case AdFormat.Banner:
                case AdFormat.Rectangle:
                    return new NullBannerAd();
                case AdFormat.AppOpen:
                case AdFormat.RewardedInterstitial:
                case AdFormat.Interstitial:
                case AdFormat.Rewarded:
                    return new NullFullScreenAd();
                case AdFormat.Null:
                    return new NullAd();
            }
            throw new ArgumentException();
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
            return new NullAd();
        }
    }

    internal interface IAdConfig {
        AdFormat Format { get; }
        IAd CreateAd(INetworkConfigManager manager);
    }

    internal static class AdConfig {
        public static IAdConfig Parse(JSONNode node) {
            var format = AdsConfigUtils.ParseAdFormat(node["format"].Value);
            switch (format) {
                case AdFormat.Banner: return new BannerConfig(node);
                case AdFormat.Rectangle: return new RectangleConfig(node);
                case AdFormat.AppOpen: return new AppOpenConfig(node);
                case AdFormat.Interstitial: return new InterstitialConfig(node);
                case AdFormat.RewardedInterstitial: return new RewardedInterstitialConfig(node);
                case AdFormat.Rewarded: return new RewardedConfig(node);
                case AdFormat.Null: return new NullAdConfig();
            }
            throw new ArgumentException();
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

    internal class RewardedInterstitialConfig : IAdConfig {
        private readonly int _interval;
        private readonly IAdInstanceConfig<IFullScreenAd> _instance;

        public RewardedInterstitialConfig(JSONNode node) {
            _interval = node.HasKey("interval") ? node["interval"].AsInt : 0;
            _instance = AdInstanceConfig<IFullScreenAd>.Parse<MultiFullScreenAd>(AdFormat.RewardedInterstitial,
                node["instance"]);
        }

        public AdFormat Format => AdFormat.RewardedInterstitial;

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

    internal class NullAdConfig : IAdConfig {
        public AdFormat Format => AdFormat.Null;

        public IAd CreateAd(INetworkConfigManager manager) {
            return new NullAd();
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
            var network = node["network"].Value;
            _network = AdsConfigUtils.ParseNetwork(network);
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

        public void OpenTestSuite(Network network) {
            _networkManager.OpenTestSuite(network);
        }

        public IAd CreateAd(AdFormat format) {
            return _adManager.CreateAd(format);
        }
    }
}