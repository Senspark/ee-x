using System;
using System.Collections.Generic;
using System.Threading.Tasks;

using UnityEngine;
using UnityEngine.Assertions;

namespace EE.Internal {
    using Destroyer = Action;

    internal class FacebookAds : IFacebookAds {
        private const string kTag = nameof(FacebookAds);
        private const string kPrefix = "FacebookAdsBridge";
        private const string kInitialize = kPrefix + "Initialize";
        private const string kGetTestDeviceHash = kPrefix + "GetTestDeviceHash";
        private const string kAddTestDevice = kPrefix + "AddTestDevice";
        private const string kClearTestDevices = kPrefix + "ClearTestDevices";
        private const string kGetBannerAdSize = kPrefix + "GetBannerAdSize";
        private const string kCreateBannerAd = kPrefix + "CreateBannerAd";
        private const string kDestroyBannerAd = kPrefix + "DestroyBannerAd";
        private const string kCreateNativeAd = kPrefix + "CreateNativeAd";
        private const string kDestroyNativeAd = kPrefix + "DestroyNativeAd";
        private const string kCreateInterstitialAd = kPrefix + "CreateInterstitialAd";
        private const string kDestroyInterstitialAd = kPrefix + "DestroyInterstitialAd";
        private const string kCreateRewardedAd = kPrefix + "CreateRewardedAd";
        private const string kDestroyRewardedAd = kPrefix + "DestroyRewardedAd";

        private readonly IMessageBridge _bridge;
        private readonly ILogger _logger;
        private readonly Destroyer _destroyer;
        private readonly Dictionary<string, IAd> _ads;
        private readonly IAsyncHelper<FullScreenAdResult> _displayer;

        public FacebookAds(IMessageBridge bridge, ILogger logger, Destroyer destroyer) {
            _bridge = bridge;
            _logger = logger;
            _destroyer = destroyer;
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

        public string GetTestDeviceHash() {
            return _bridge.Call(kGetTestDeviceHash);
        }

        public void AddTestDevice(string hash) {
            _bridge.Call(kAddTestDevice, hash);
        }

        public void ClearTestDevices() {
            _bridge.Call(kClearTestDevices);
        }

        [Serializable]
        private struct GetBannerAdSizeResponse {
            public int width;
            public int height;
        }

        [Serializable]
        private struct CreateBannerAdRequest {
            public string adId;
            public int adSize;
        }

        private (int, int) GetBannerAdSize(FacebookBannerAdSize adSize) {
            var response = _bridge.Call(kGetBannerAdSize, ((int) adSize).ToString());
            var json = JsonUtility.FromJson<GetBannerAdSizeResponse>(response);
            return (json.width, json.height);
        }

        public IBannerAd CreateBannerAd(string adId, FacebookBannerAdSize adSize) {
            _logger.Debug($"${kTag}: {nameof(CreateBannerAd)}: id = {adId} size = {adSize}");
            if (_ads.TryGetValue(adId, out var result)) {
                return result as IBannerAd;
            }
            var request = new CreateBannerAdRequest {
                adId = adId,
                adSize = (int) adSize,
            };
            var response = _bridge.Call(kCreateBannerAd, JsonUtility.ToJson(request));
            if (!Utils.ToBool(response)) {
                Assert.IsTrue(false);
                return null;
            }
            var size = GetBannerAdSize(adSize);
            var ad = new GuardedBannerAd(new DefaultBannerAd("FacebookBannerAd", _bridge, _logger,
                () => DestroyAd(kDestroyBannerAd, adId), adId, size));
            _ads.Add(adId, ad);
            return ad;
        }

        public IFullScreenAd CreateInterstitialAd(string adId) {
            return CreateFullScreenAd(kCreateInterstitialAd, adId,
                () => new DefaultFullScreenAd("FacebookInterstitialAd", _bridge, _logger, _displayer,
                    () => DestroyAd(kDestroyInterstitialAd, adId),
                    _ => FullScreenAdResult.Completed,
                    adId));
        }

        public IFullScreenAd CreateRewardedAd(string adId) {
            return CreateFullScreenAd(kCreateRewardedAd, adId,
                () => new DefaultFullScreenAd("FacebookRewardedAd", _bridge, _logger, _displayer,
                    () => DestroyAd(kDestroyRewardedAd, adId),
                    message => Utils.ToBool(message)
                        ? FullScreenAdResult.Completed
                        : FullScreenAdResult.Canceled,
                    adId));
        }

        private IFullScreenAd CreateFullScreenAd(string handlerId, string adId, Func<IFullScreenAd> creator) {
            _logger.Debug($"${kTag}: {nameof(CreateFullScreenAd)}: id = {adId}");
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

        private bool DestroyAd(string handlerId, string adId) {
            _logger.Debug($"${kTag}: {nameof(DestroyAd)}: id = {adId}");
            if (!_ads.ContainsKey(adId)) {
                return false;
            }
            var response = _bridge.Call(handlerId, adId);
            if (!Utils.ToBool(response)) {
                Assert.IsTrue(false);
                return false;
            }
            _ads.Remove(adId);
            return true;
        }
    }
}