using System;
using System.Collections.Generic;
using System.Threading.Tasks;

using UnityEngine;
using UnityEngine.Assertions;

namespace EE.Internal {
    using Destroyer = Action;

    internal class AdMob : IAdMob {
        private const string kTag = nameof(AdMob);
        private const string kPrefix = "AdMobBridge";
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

        public AdMob(IMessageBridge bridge, ILogger logger, Destroyer destroyer) {
            _bridge = bridge;
            _logger = logger;
            _destroyer = destroyer;
            _logger.Debug($"{kTag}: constructor");
            _network = "ad_mob";
            _ads = new Dictionary<string, IAd>();
            _displayer = MediationManager.Instance.AdDisplayer;
        }

        public void Destroy() {
            _logger.Debug($"{kTag}: constructor");
            foreach (var ad in _ads.Values) {
                ad.Destroy();
            }
            _ads.Clear();
            _destroyer();
        }

        public async Task<bool> Initialize() {
            var response = await _bridge.CallAsync(kInitialize);
            return Utils.ToBool(response);
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

        private (int, int) GetBannerAdSize(AdMobBannerAdSize adSize) {
            var response = _bridge.Call(kGetBannerAdSize, ((int) adSize).ToString());
            var json = JsonUtility.FromJson<GetBannerAdSizeResponse>(response);
            return (json.width, json.height);
        }

        [Serializable]
        private struct CreateBannerAdRequest {
            public string adId;
            public int adSize;
        }

        public IBannerAd CreateBannerAd(string adId, AdMobBannerAdSize adSize) {
            _logger.Debug($"{kTag}: {nameof(CreateBannerAd)}: id = {adId} size = {adSize}");
            if (_ads.TryGetValue(adId, out var result)) {
                return result as IBannerAd;
            }
            var request = new CreateBannerAdRequest {
                adId = adId,
                adSize = (int) adSize
            };
            var response = _bridge.Call(kCreateBannerAd, JsonUtility.ToJson(request));
            if (!Utils.ToBool(response)) {
                Assert.IsTrue(false);
                return null;
            }
            var size = GetBannerAdSize(adSize);
            var ad = new GuardedBannerAd(new DefaultBannerAd("AdMobBannerAd", _bridge, _logger,
                () => DestroyAd(adId), _network, adId, size));
            _ads.Add(adId, ad);
            return ad;
        }

        public IFullScreenAd CreateAppOpenAd(string adId) {
            return CreateFullScreenAd(kCreateAppOpenAd, adId,
                () => new DefaultFullScreenAd("AdMobAppOpenAd", _bridge, _logger, _displayer,
                    () => DestroyAd(adId),
                    _ => AdResult.Completed,
                    _network, adId));
        }

        public IFullScreenAd CreateInterstitialAd(string adId) {
            return CreateFullScreenAd(kCreateInterstitialAd, adId,
                () => new DefaultFullScreenAd("AdMobInterstitialAd", _bridge, _logger, _displayer,
                    () => DestroyAd(adId),
                    _ => AdResult.Completed,
                    _network, adId));
        }

        public IFullScreenAd CreateRewardedInterstitialAd(string adId) {
            return CreateFullScreenAd(kCreateRewardedInterstitialAd, adId,
                () => new DefaultFullScreenAd("AdMobRewardedInterstitialAd", _bridge, _logger, _displayer,
                    () => DestroyAd(adId),
                    message => Utils.ToBool(message)
                        ? AdResult.Completed
                        : AdResult.Canceled,
                    _network, adId));
        }

        public IFullScreenAd CreateRewardedAd(string adId) {
            return CreateFullScreenAd(kCreateRewardedAd, adId,
                () => new DefaultFullScreenAd("AdMobRewardedAd", _bridge, _logger, _displayer,
                    () => DestroyAd(adId),
                    message => Utils.ToBool(message)
                        ? AdResult.Completed
                        : AdResult.Canceled,
                    _network, adId));
        }

        private IFullScreenAd CreateFullScreenAd(string handlerId, string adId, Func<IFullScreenAd> creator) {
            _logger.Debug($"{kTag}: {nameof(CreateFullScreenAd)}: id = {adId}");
            if (_ads.TryGetValue(adId, out var result)) {
                return result as IFullScreenAd;
            }
            var response = _bridge.Call(handlerId, adId);
            if (!Utils.ToBool(response)) {
                Assert.IsTrue(false);
                return null;
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