using System;
using System.Collections.Generic;
using System.Threading.Tasks;

using EE.Internal;

namespace EE {
    public class DefaultAdsManager : IAdsManager {
        private class AdEntry {
            public IFullScreenAd Ad;
            public Action<IFullScreenAd> Initializer;
        }

        private Task<bool> _initializer;
        private readonly IAnalyticsManager _analyticsManager;
        private readonly ICapper _displayCapper;
        private readonly AdsConfig _config;
        private readonly Dictionary<AdFormat, LazyBannerAd> _bannerAds;
        private readonly Dictionary<AdFormat, AdEntry> _fullScreenAds;

        public DefaultAdsManager(string configJson) :
            this(configJson, new NullAnalyticsManager(new NullLogManager())) {
        }

        public DefaultAdsManager(string configJson, IAnalyticsManager analyticsManager) {
            _analyticsManager = analyticsManager;
            _displayCapper = new Capper(0.1f);
            _config = AdsConfig.Parse(configJson);
            _bannerAds = new Dictionary<AdFormat, LazyBannerAd> {
                [AdFormat.Banner] = new LazyBannerAd(),
                [AdFormat.Rectangle] = new LazyBannerAd()
            };
            _fullScreenAds = new Dictionary<AdFormat, AdEntry> {
                [AdFormat.AppOpen] = CreateFullScreenAd(AdFormat.AppOpen),
                [AdFormat.Interstitial] = CreateFullScreenAd(AdFormat.Interstitial),
                [AdFormat.RewardedInterstitial] = CreateFullScreenAd(AdFormat.RewardedInterstitial),
                [AdFormat.Rewarded] = CreateFullScreenAd(AdFormat.Rewarded)
            };
        }

        public Task<bool> Initialize() => _initializer = _initializer ?? (_initializer = InitializeImpl());

        public void Destroy() {
        }

        private async Task<bool> InitializeImpl() {
            await _config.Initialize();
            InitializeBannerAd(AdFormat.Banner);
            InitializeBannerAd(AdFormat.Rectangle);
            InitializeFullScreenAd(AdFormat.AppOpen);
            InitializeFullScreenAd(AdFormat.Interstitial);
            InitializeFullScreenAd(AdFormat.RewardedInterstitial);
            InitializeFullScreenAd(AdFormat.Rewarded);
            return true;
        }

        private AdEntry CreateFullScreenAd(AdFormat format) {
            var ad = new LazyFullScreenAd(_displayCapper);
            var analyticsAd = new AnalyticsFullScreenAd(ad, _analyticsManager, format);
            void Initializer(IFullScreenAd item) => ad.Ad = item;
            return new AdEntry {
                Ad = analyticsAd,
                Initializer = Initializer
            };
        }

        private void InitializeBannerAd(AdFormat format) {
            if (_config.CreateAd(format) is IBannerAd ad) {
                // OK.
            } else {
                return;
            }
            _bannerAds[format].Ad = ad;
        }

        private void InitializeFullScreenAd(AdFormat format) {
            if (_config.CreateAd(format) is IFullScreenAd ad) {
                // OK.
            } else {
                return;
            }
            _fullScreenAds[format].Initializer(ad);
        }

        public void OpenTestSuite() {
            _config.OpenTestSuite(AdNetwork.AdMob);
        }

        public IBannerAd BannerAd => _bannerAds[AdFormat.Banner];
        public IBannerAd RectangleAd => _bannerAds[AdFormat.Rectangle];
        public IFullScreenAd AppOpenAd => _fullScreenAds[AdFormat.AppOpen].Ad;
        public IFullScreenAd InterstitialAd => _fullScreenAds[AdFormat.Interstitial].Ad;
        public IFullScreenAd RewardedInterstitialAd => _fullScreenAds[AdFormat.RewardedInterstitial].Ad;
        public IFullScreenAd RewardedAd => _fullScreenAds[AdFormat.Rewarded].Ad;
    }
}