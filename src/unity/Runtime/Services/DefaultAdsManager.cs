using System;
using System.Collections.Generic;
using System.Threading.Tasks;

using EE.Internal;

namespace EE {
    public class DefaultAdsManager : ObserverManager<AdsObserver>, IAdsManager {
        private Task<bool> _initializer;
        private bool _initialized;
        private readonly AdsConfig _config;
        private LazyAdView _bannerAd;
        private readonly Dictionary<AdFormat, GenericAd> _fullScreenAds;
        private readonly ObserverHandle _handle;

        public DefaultAdsManager(string configJson) {
            _config = AdsConfig.Parse(configJson);
            _fullScreenAds = new Dictionary<AdFormat, GenericAd>();
            _handle = new ObserverHandle();
        }

        public Task<bool> Initialize() => _initializer ?? (_initializer = InitializeImpl());

        private async Task<bool> InitializeImpl() {
            _bannerAd = new LazyAdView();
            await _config.Initialize();
            InitializeBannerAd();
            InitializeFullScreenAd(AdFormat.AppOpen);
            InitializeFullScreenAd(AdFormat.Interstitial);
            InitializeFullScreenAd(AdFormat.Rewarded);
            _initialized = true;
            return true;
        }

        private void InitializeBannerAd() {
            if (_config.CreateAd(AdFormat.Banner) is IAdView ad) {
                // OK.
            } else {
                return;
            }
            _bannerAd.Ad = ad;
            _handle.Bind(_bannerAd)
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

        public bool IsBannerAdVisible {
            get => _bannerAd.IsVisible;
            set => _bannerAd.IsVisible = value;
        }

        public (float, float) BannerAdAnchor {
            get => _bannerAd.Anchor;
            set => _bannerAd.Anchor = value;
        }

        public (float, float) BannerAdPosition {
            get => _bannerAd.Position;
            set => _bannerAd.Position = value;
        }

        public (float, float) BannerAdSize {
            get => _bannerAd.Size;
            set => _bannerAd.Size = value;
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
            return await ad.Show();
        }
    }
}