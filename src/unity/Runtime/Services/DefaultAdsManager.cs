using System.Collections.Generic;
using System.Threading.Tasks;

using EE.Internal;

namespace EE {
    public class DefaultAdsManager : IAdsManager {
        private Task<bool> _initializer;
        private bool _initialized;
        private readonly ICapper _displayCapper;
        private readonly AdsConfig _config;
        private readonly Dictionary<AdFormat, LazyBannerAd> _bannerAds;
        private readonly Dictionary<AdFormat, LazyFullScreenAd> _fullScreenAds;

        public DefaultAdsManager(string configJson) {
            _initialized = false;
            _displayCapper = new Capper(0.1f);
            _config = AdsConfig.Parse(configJson);
            _bannerAds = new Dictionary<AdFormat, LazyBannerAd>();
            _fullScreenAds = new Dictionary<AdFormat, LazyFullScreenAd>();
        }

        public Task<bool> Initialize() => _initializer ??= InitializeImpl();

        public void Destroy() {
        }

        private async Task<bool> InitializeImpl() {
            _bannerAds[AdFormat.Banner] = new LazyBannerAd();
            _bannerAds[AdFormat.Rectangle] = new LazyBannerAd();
            _fullScreenAds[AdFormat.AppOpen] = new LazyFullScreenAd(_displayCapper);
            _fullScreenAds[AdFormat.Interstitial] = new LazyFullScreenAd(_displayCapper);
            _fullScreenAds[AdFormat.RewardedInterstitial] = new LazyFullScreenAd(_displayCapper);
            _fullScreenAds[AdFormat.Rewarded] = new LazyFullScreenAd(_displayCapper);
            await _config.Initialize();
            InitializeBannerAd(AdFormat.Banner);
            InitializeBannerAd(AdFormat.Rectangle);
            InitializeFullScreenAd(AdFormat.AppOpen);
            InitializeFullScreenAd(AdFormat.Interstitial);
            InitializeFullScreenAd(AdFormat.RewardedInterstitial);
            InitializeFullScreenAd(AdFormat.Rewarded);
            _initialized = true;
            return true;
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
            _fullScreenAds[format].Ad = ad;
        }

        public void OpenTestSuite() {
            _config.OpenTestSuite(Network.AdMob);
        }

        public IBannerAd BannerAd => _bannerAds[AdFormat.Banner];
        public IBannerAd RectangleAd => _bannerAds[AdFormat.Rectangle];
        public IFullScreenAd AppOpenAd => _fullScreenAds[AdFormat.AppOpen];
        public IFullScreenAd InterstitialAd => _fullScreenAds[AdFormat.Interstitial];
        public IFullScreenAd RewardedInterstitialAd => _fullScreenAds[AdFormat.RewardedInterstitial];
        public IFullScreenAd RewardedAd => _fullScreenAds[AdFormat.Rewarded];
    }
}