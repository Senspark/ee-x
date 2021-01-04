using System;
using System.Threading.Tasks;

namespace EE {
    public class DefaultAdsManager : IAdsManager {
        private Task<bool> _initializer;
        private bool _initialized;

        private readonly AdsConfig _config;

        private IAdMob _adMob;
        private IFacebookAds _facebookAds;
        private IIronSource _ironSource;
        private IUnityAds _unityAds;
        private IAdView _bannerAd;
        private IInterstitialAd _interstitialAd;
        private IRewardedAd _rewardedAd;

        private bool _isInterstitialAdCapped;
        private int _interstitialAdInterval;
        private bool _isBannerAdVisible;
        private (float, float) _bannerAdAnchor;
        private (float, float) _bannerAdPosition;
        private (float, float)? _bannerAdSize;

        public DefaultAdsManager(string configJson) {
            _config = AdsConfig.Parse(configJson);
        }

        public Task<bool> Initialize() => _initializer ?? (_initializer = InitializeImpl());

        private async Task<bool> InitializeImpl() {
            await InitializeNetworks();
            CreateAds();
            _initialized = true;
            return true;
        }

        private async Task InitializeNetworks() {
            foreach (var network in _config.Networks) {
                switch (network) {
                    case AdsConfig.AdMobConfig _:
                        _adMob = PluginManager.CreatePlugin<IAdMob>();
                        await _adMob.Initialize();
                        break;
                    case AdsConfig.FacebookAdsConfig _:
                        _facebookAds = PluginManager.CreatePlugin<IFacebookAds>();
                        await _facebookAds.Initialize();
                        break;
                    case AdsConfig.IronSourceConfig config:
                        _ironSource = PluginManager.CreatePlugin<IIronSource>();
                        await _ironSource.Initialize(config.AppId);
                        break;
                    case AdsConfig.UnityAdsConfig config:
                        _unityAds = PluginManager.CreatePlugin<IUnityAds>();
                        await _unityAds.Initialize(config.AppId, false);
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
        }

        private void CreateAds() {
            foreach (var ad in _config.Ads) {
                switch (ad) {
                    case AdsConfig.BannerConfig config:
                        CreateAd(config);
                        break;
                    case AdsConfig.InterstitialConfig config:
                        CreateAd(config);
                        break;
                    case AdsConfig.RewardedConfig config:
                        CreateAd(config);
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
        }

        private void CreateAd(AdsConfig.BannerConfig config) {
            if (_bannerAd != null) {
                return;
            }
            _bannerAd = new UnityAdView(CreateBannerAd(config.Instance)) {
                IsVisible = _isBannerAdVisible,
                Anchor = _bannerAdAnchor,
                Position = _bannerAdPosition
            };
            if (_bannerAdSize.HasValue) {
                _bannerAd.Size = _bannerAdSize.Value;
            }
            Utils.NoAwait(async () => {
                var result = await _bannerAd.Load();
                if (result && _bannerAd.IsVisible) {
                    _bannerAd.IsVisible = false;
                    _bannerAd.IsVisible = true;
                }
            });
        }

        private void CreateAd(AdsConfig.InterstitialConfig config) {
            if (_interstitialAd != null) {
                return;
            }
            _interstitialAd = CreateInterstitialAd(config.Instance);
            Utils.NoAwait(_interstitialAd.Load);
            _interstitialAdInterval = config.Interval;
            _isInterstitialAdCapped = true;
            Utils.NoAwait(async () => {
                await Task.Delay(_interstitialAdInterval * 1000);
                _isInterstitialAdCapped = false;
            });
        }

        private void CreateAd(AdsConfig.RewardedConfig config) {
            if (_rewardedAd != null) {
                return;
            }
            _rewardedAd = CreateRewardedAd(config.Instance);
            Utils.NoAwait(_rewardedAd.Load);
        }

        private IAdView CreateBannerAd(AdsConfig.AdInstanceConfig config) {
            switch (config) {
                case AdsConfig.SingleInstanceConfig instance:
                    return CreateBannerAd(instance);
                case AdsConfig.WaterfallInstanceConfig instance:
                    return CreateBannerAd(instance);
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }

        private IAdView CreateBannerAd(AdsConfig.SingleInstanceConfig config) {
            switch (config.Network) {
                case AdsConfig.Network.AdMob:
                    return _adMob.CreateBannerAd(config.Id, AdMobBannerAdSize.Normal);
                case AdsConfig.Network.FacebookAds:
                    return _facebookAds.CreateBannerAd(config.Id, FacebookBannerAdSize.BannerHeight50);
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }

        private IAdView CreateBannerAd(AdsConfig.WaterfallInstanceConfig config) {
            var ad = new MultiAdView();
            foreach (var instance in config.Instances) {
                ad.AddItem(CreateBannerAd(instance));
            }
            return ad;
        }

        private IInterstitialAd CreateInterstitialAd(AdsConfig.AdInstanceConfig config) {
            switch (config) {
                case AdsConfig.SingleInstanceConfig instance:
                    return CreateInterstitialAd(instance);
                case AdsConfig.WaterfallInstanceConfig instance:
                    return CreateInterstitialAd(instance);
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }

        private IInterstitialAd CreateInterstitialAd(AdsConfig.SingleInstanceConfig config) {
            switch (config.Network) {
                case AdsConfig.Network.AdMob:
                    return _adMob.CreateInterstitialAd(config.Id);
                case AdsConfig.Network.FacebookAds:
                    return _facebookAds.CreateInterstitialAd(config.Id);
                case AdsConfig.Network.IronSource:
                    return _ironSource.CreateInterstitialAd(config.Id);
                case AdsConfig.Network.UnityAds:
                    return _unityAds.CreateInterstitialAd(config.Id);
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }

        private IInterstitialAd CreateInterstitialAd(AdsConfig.WaterfallInstanceConfig config) {
            var ad = new MultiInterstitialAd();
            foreach (var instance in config.Instances) {
                ad.AddItem(CreateInterstitialAd(instance));
            }
            return ad;
        }

        private IRewardedAd CreateRewardedAd(AdsConfig.AdInstanceConfig config) {
            switch (config) {
                case AdsConfig.SingleInstanceConfig instance:
                    return CreateRewardedAd(instance);
                case AdsConfig.WaterfallInstanceConfig instance:
                    return CreateRewardedAd(instance);
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }

        private IRewardedAd CreateRewardedAd(AdsConfig.SingleInstanceConfig config) {
            switch (config.Network) {
                case AdsConfig.Network.AdMob:
                    return _adMob.CreateRewardedAd(config.Id);
                case AdsConfig.Network.FacebookAds:
                    return _facebookAds.CreateRewardedAd(config.Id);
                case AdsConfig.Network.IronSource:
                    return _ironSource.CreateRewardedAd(config.Id);
                case AdsConfig.Network.UnityAds:
                    return _unityAds.CreateRewardedAd(config.Id);
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }

        private IRewardedAd CreateRewardedAd(AdsConfig.WaterfallInstanceConfig config) {
            var ad = new MultiRewardedAd();
            foreach (var instance in config.Instances) {
                ad.AddItem(CreateRewardedAd(instance));
            }
            return ad;
        }

        public bool IsBannerAdVisible {
            get => _isBannerAdVisible;
            set {
                _isBannerAdVisible = value;
                if (_bannerAd != null) {
                    _bannerAd.IsVisible = value;
                }
            }
        }

        public (float, float) BannerAdAnchor {
            get => _bannerAdAnchor;
            set {
                _bannerAdAnchor = value;
                if (_bannerAd != null) {
                    _bannerAd.Anchor = value;
                }
            }
        }

        public (float, float) BannerAdPosition {
            get => _bannerAdPosition;
            set {
                _bannerAdPosition = value;
                if (_bannerAd != null) {
                    _bannerAd.Position = value;
                }
            }
        }

        public (float, float) BannerAdSize {
            get {
                if (_bannerAd != null) {
                    return _bannerAd.Size;
                }
                return _bannerAdSize ?? (0, 0);
            }
            set {
                _bannerAdSize = value;
                if (_bannerAd != null) {
                    _bannerAd.Size = value;
                }
            }
        }

        public async Task<AdResult> ShowInterstitialAd() {
            if (!_initialized) {
                return AdResult.NotInitialized;
            }
            if (_interstitialAd == null) {
                return AdResult.NotConfigured;
            }
            if (_interstitialAdInterval > 0 && _isInterstitialAdCapped) {
                return AdResult.Capped;
            }
            if (_interstitialAd.IsLoaded) {
                var hasInternet = await TestConnection(0.2f);
                if (!hasInternet) {
                    return AdResult.NoInternet;
                }
            } else {
                Utils.NoAwait(_interstitialAd.Load);
                var hasInternet = false;
                await Task.WhenAll(
                    Task.Delay(100),
                    ((Func<Task>) (async () => { //
                        hasInternet = await TestConnection(0.2f);
                    }))());
                if (!hasInternet) {
                    return AdResult.NoInternet;
                }
            }
            if (!_interstitialAd.IsLoaded) {
                return AdResult.NotLoaded;
            }
            var result = await _interstitialAd.Show();
            Utils.NoAwait(_interstitialAd.Load);
            switch (result) {
                case true:
                    if (_interstitialAdInterval > 0) {
                        _isInterstitialAdCapped = true;
                        Utils.NoAwait(async () => {
                            await Task.Delay(_interstitialAdInterval * 1000);
                            _isInterstitialAdCapped = false;
                        });
                    }
                    return AdResult.Completed;
                default:
                    return AdResult.Failed;
            }
        }

        public async Task<AdResult> ShowRewardedAd() {
            if (!_initialized) {
                return AdResult.NotInitialized;
            }
            if (_rewardedAd == null) {
                return AdResult.NotConfigured;
            }
            if (_rewardedAd.IsLoaded) {
                var hasInternet = await TestConnection(0.2f);
                if (!hasInternet) {
                    return AdResult.NoInternet;
                }
            } else {
                Utils.NoAwait(_rewardedAd.Load);
                var hasInternet = false;
                await Task.WhenAll(
                    Task.Delay(100),
                    ((Func<Task>) (async () => { //
                        hasInternet = await TestConnection(0.2f);
                    }))());
                if (!hasInternet) {
                    return AdResult.NoInternet;
                }
            }
            if (!_rewardedAd.IsLoaded) {
                return AdResult.NotLoaded;
            }
            var result = await _rewardedAd.Show();
            Utils.NoAwait(_rewardedAd.Load);
            switch (result) {
                case IRewardedAdResult.Failed:
                    return AdResult.Failed;
                case IRewardedAdResult.Canceled:
                    return AdResult.Canceled;
                case IRewardedAdResult.Completed:
                    return AdResult.Completed;
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }

        private static async Task<bool> TestConnection(float timeOut) {
            return await Platform.TestConnection("www.google.com", timeOut);
        }
    }
}