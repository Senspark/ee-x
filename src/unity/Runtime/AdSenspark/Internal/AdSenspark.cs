using System;
using System.Collections.Generic;
using System.Threading.Tasks;

using UnityEngine.Assertions;

namespace EE.Internal {
    using Destroyer = Action;

    internal class AdSenspark : IAdSenspark {
        private const string kTag = nameof(AdSenspark);
        private const string kPrefix = "AdSensparkBridge";
        private const string kInitialize = kPrefix + "Initialize";
        private const string kGetEmulatorTestDeviceHash = kPrefix + "GetEmulatorTestDeviceHash";
        private const string kAddTestDevice = kPrefix + "AddTestDevice";
        private const string kGetBannerAdSize = kPrefix + "GetBannerAdSize";
        private const string kOpenTestSuite = kPrefix + "OpenTestSuite";
        private const string kCreateBannerAd = kPrefix + "CreateBannerAd";
        private const string kCreateNativeAd = kPrefix + "CreateNativeAd";
        private const string kCreateAppOpenAd = kPrefix + "CreateAppOpenAd";
        private const string kCreateInterstitialAd = kPrefix + "CreateInterstitialAd";
        private const string kCreateRewardedInterstitialAd = kPrefix + "CreateRewardedInterstitialAd";
        private const string kCreateRewardedAd = kPrefix + "CreateRewardedAd";
        private const string kDestroyAd = kPrefix + "DestroyAd";

        private readonly IMessageBridge _bridge;
        private readonly ILogger _logger;
        private readonly Destroyer _destroyer;
        private readonly string _network;
        private readonly Dictionary<string, IAd> _ads;
        private readonly IAsyncHelper<AdResult> _displayer;
        private readonly AdSensparkResourceManager _mgResourceManager;

        public AdSenspark(IMessageBridge bridge, ILogger logger, Destroyer destroyer) {
            _bridge = bridge;
            _logger = logger;
            _destroyer = destroyer;
            _logger.Debug($"{kTag}: constructor");
            _network = "ad_senspark";
            _ads = new Dictionary<string, IAd>();
            _displayer = MediationManager.Instance.AdDisplayer;
            _mgResourceManager = new AdSensparkResourceManager();
        }

        public void Destroy() {
            _logger.Debug($"{kTag}: constructor");
            foreach (var ad in _ads.Values) {
                ad.Destroy();
            }
            _ads.Clear();
            _mgResourceManager.OnDestroy();
            _destroyer();
        }

        public async Task<bool> Initialize(string url) {
            return await _mgResourceManager.Initialize(url);
        }

        public string GetEmulatorTestDeviceHash() {
            return _bridge.Call(kGetEmulatorTestDeviceHash);
        }

        public void AddTestDevice(string hash) {
            _bridge.Call(kAddTestDevice, hash);
        }

        public void OpenTestSuite() {
            _bridge.Call(kOpenTestSuite);
        }

        [Serializable]
        private struct GetBannerAdSizeResponse {
            public int width;
            public int height;
        }

        private (int, int) GetBannerAdSize(AdSensparkBannerAdSize adSize) {
            float widthAd, heightAd;
            switch (adSize) {
                case AdSensparkBannerAdSize.Normal:
                    widthAd = 300;
                    heightAd = 50;
                    break;
                case AdSensparkBannerAdSize.MediumRectangle:
                    widthAd = 300;
                    heightAd = 250;
                    break;
                default:
                    widthAd = 300;
                    heightAd = 50;
                    break;
            }
            widthAd = Platform.GetDensity() * widthAd; // pixel = ratio * 300; // Chuyển đổi từ DP sang Pixel.
            heightAd = Platform.GetDensity() * heightAd;
            return ((int) widthAd, (int) heightAd);
        }

        [Serializable]
        private struct CreateBannerAdRequest {
            public string adId;
            public int adSize;
        }

        public IBannerAd CreateBannerAd(string adId, AdSensparkBannerAdSize adSize) {
            _logger.Debug($"{kTag}: {nameof(CreateBannerAd)}: id = {adId} size = {adSize}");
            if (_ads.TryGetValue(adId, out var result)) {
                return result as IBannerAd;
            }
            
            var size = GetBannerAdSize(adSize);
            var ad = new GuardedBannerAd(new AdSensparkDefaultBanner("SensparkBannerAd", _bridge, _logger,
                () => DestroyAd(adId), _network, adId, AdFormat.Banner, size, _mgResourceManager));
            _ads.Add(adId, ad);
            return ad;
        }

        public IFullScreenAd CreateAppOpenAd(string adId) {
            return new NullFullScreenAd();
        }

        public IFullScreenAd CreateInterstitialAd(string adId) {
            return CreateFullScreenAd(kCreateInterstitialAd, adId,
                () => new AdSensparkDefaultFullScreenAd("SensparkInterstitialAd", _bridge, _logger, _displayer,
                    () => DestroyAd(adId),
                    _ => AdResult.Completed,
                    _network, adId, AdFormat.Interstitial, _mgResourceManager));
        }

        public IFullScreenAd CreateRewardedInterstitialAd(string adId) {
            return new NullFullScreenAd();
        }

        public IFullScreenAd CreateRewardedAd(string adId) {
            return CreateFullScreenAd(kCreateRewardedAd, adId,
                () => new AdSensparkDefaultFullScreenAd("SensparkRewardedAd", _bridge, _logger, _displayer,
                    () => DestroyAd(adId),
                    message => Utils.ToBool(message)
                        ? AdResult.Completed
                        : AdResult.Canceled,
                    _network, adId, AdFormat.Rewarded, _mgResourceManager));
        }

        private IFullScreenAd CreateFullScreenAd(string handlerId, string adId, Func<IFullScreenAd> creator) {
            _logger.Debug($"{kTag}: {nameof(CreateFullScreenAd)}: id = {adId}");
            if (_ads.TryGetValue(adId, out var result)) {
                return result as IFullScreenAd;
            }
            var ad = new GuardedFullScreenAd(creator());
            _ads.Add(adId, ad);
            return ad;
        }

        private bool DestroyAd(string adId) {
            _logger.Debug($"{kTag}: {nameof(DestroyAd)}: id = {adId}");
            if (!_ads.ContainsKey(adId)) {
                return false;
            }
            var response = _bridge.Call(kDestroyAd, adId);
            if (!Utils.ToBool(response)) {
                Assert.IsTrue(false);
                return false;
            }
            _ads.Remove(adId);
            return true;
        }
    }
}