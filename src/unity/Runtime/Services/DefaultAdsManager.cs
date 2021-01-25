using System.Collections.Generic;
using System.Threading.Tasks;

using EE.Internal;

namespace EE {
    public class DefaultAdsManager : ObserverManager<AdsObserver>, IAdsManager {
        private Task<bool> _initializer;
        private bool _initialized;
        private bool _fullScreenAdCapped;
        private readonly AdsConfig _config;
        private readonly Dictionary<AdFormat, LazyBannerAd> _bannerAds;
        private readonly Dictionary<AdFormat, GenericAd> _fullScreenAds;
        private readonly ObserverHandle _handle;

        public DefaultAdsManager(string configJson) {
            _initialized = false;
            _fullScreenAdCapped = false;
            _config = AdsConfig.Parse(configJson);
            _bannerAds = new Dictionary<AdFormat, LazyBannerAd>();
            _fullScreenAds = new Dictionary<AdFormat, GenericAd>();
            _handle = new ObserverHandle();
        }

        public Task<bool> Initialize() => _initializer ?? (_initializer = InitializeImpl());

        private async Task<bool> InitializeImpl() {
            _bannerAds[AdFormat.Banner] = new LazyBannerAd();
            _bannerAds[AdFormat.Rectangle] = new LazyBannerAd();
            await _config.Initialize();
            InitializeBannerAd(AdFormat.Banner);
            InitializeBannerAd(AdFormat.Rectangle);
            InitializeFullScreenAd(AdFormat.AppOpen);
            InitializeFullScreenAd(AdFormat.Interstitial);
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
            _handle.Bind(_bannerAds[format])
                .AddObserver(new AdObserver {
                    OnClicked = () => DispatchEvent(observer => observer.OnClicked?.Invoke())
                });
        }

        private void InitializeFullScreenAd(AdFormat format) {
            if (_config.CreateAd(format) is GenericAd ad) {
                // OK.
            } else {
                return;
            }
            _fullScreenAds[format] = ad;
            _handle.Bind(ad)
                .AddObserver(new AdObserver {
                    OnClicked = () => DispatchEvent(observer => observer.OnClicked?.Invoke())
                });
        }

        public bool IsBannerAdLoaded => _bannerAds[AdFormat.Banner].IsLoaded;

        public bool IsBannerAdVisible {
            get => _bannerAds[AdFormat.Banner].IsVisible;
            set => _bannerAds[AdFormat.Banner].IsVisible = value;
        }

        public (float, float) BannerAdAnchor {
            get => _bannerAds[AdFormat.Banner].Anchor;
            set => _bannerAds[AdFormat.Banner].Anchor = value;
        }

        public (float, float) BannerAdPosition {
            get => _bannerAds[AdFormat.Banner].Position;
            set => _bannerAds[AdFormat.Banner].Position = value;
        }

        public (float, float) BannerAdSize {
            get => _bannerAds[AdFormat.Banner].Size;
            set => _bannerAds[AdFormat.Banner].Size = value;
        }

        public bool IsRectangleAdLoaded => _bannerAds[AdFormat.Rectangle].IsLoaded;

        public bool IsRectangleAdVisible {
            get => _bannerAds[AdFormat.Rectangle].IsVisible;
            set => _bannerAds[AdFormat.Rectangle].IsVisible = value;
        }

        public (float, float) RectangleAdAnchor {
            get => _bannerAds[AdFormat.Rectangle].Anchor;
            set => _bannerAds[AdFormat.Rectangle].Anchor = value;
        }

        public (float, float) RectangleAdPosition {
            get => _bannerAds[AdFormat.Rectangle].Position;
            set => _bannerAds[AdFormat.Rectangle].Position = value;
        }

        public (float, float) RectangleAdSize {
            get => _bannerAds[AdFormat.Rectangle].Size;
            set => _bannerAds[AdFormat.Rectangle].Size = value;
        }

        public async Task<AdResult> ShowAppOpenAd() {
            return await ShowFullScreenAd(AdFormat.AppOpen);
        }

        public async Task<AdResult> ShowInterstitialAd() {
            return await ShowFullScreenAd(AdFormat.Interstitial);
        }

        public async Task<AdResult> ShowRewardedAd() {
            return await ShowFullScreenAd(AdFormat.Rewarded);
        }

        private async Task<AdResult> ShowFullScreenAd(AdFormat format) {
            if (!_initialized) {
                return AdResult.NotInitialized;
            }
            if (_fullScreenAds.TryGetValue(format, out var ad)) {
                // OK.
            } else {
                return AdResult.NotConfigured;
            }
            if (_fullScreenAdCapped) {
                return AdResult.Capped;
            }
            _fullScreenAdCapped = true;
            var result = await ad.Show();
            Utils.NoAwait(async () => {
                await Task.Delay(100);
                _fullScreenAdCapped = false;
            });
            return result;
        }
    }
}