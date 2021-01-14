using System;
using System.Collections.Generic;
using System.Threading.Tasks;

using UnityEngine;
using UnityEngine.Assertions;

namespace EE.Internal {
    internal class AdMob : IAdMob {
        private const string kPrefix = "AdMobBridge";
        private const string kInitialize = kPrefix + "Initialize";
        private const string kGetEmulatorTestDeviceHash = kPrefix + "GetEmulatorTestDeviceHash";
        private const string kAddTestDevice = kPrefix + "AddTestDevice";
        private const string kGetBannerAdSize = kPrefix + "GetBannerAdSize";
        private const string kCreateBannerAd = kPrefix + "CreateBannerAd";
        private const string kDestroyBannerAd = kPrefix + "DestroyBannerAd";
        private const string kCreateNativeAd = kPrefix + "CreateNativeAd";
        private const string kDestroyNativeAd = kPrefix + "DestroyNativeAd";
        private const string kCreateAppOpenAd = kPrefix + "CreateAppOpenAd";
        private const string kDestroyAppOpenAd = kPrefix + "DestroyAppOpenAd";
        private const string kCreateInterstitialAd = kPrefix + "CreateInterstitialAd";
        private const string kDestroyInterstitialAd = kPrefix + "DestroyInterstitialAd";
        private const string kCreateRewardedAd = kPrefix + "CreateRewardedAd";
        private const string kDestroyRewardedAd = kPrefix + "DestroyRewardedAd";

        private readonly IMessageBridge _bridge;
        private readonly Dictionary<string, IAd> _ads;
        private readonly IAsyncHelper<FullScreenAdResult> _displayer;

        public AdMob(IMessageBridge bridge) {
            _bridge = bridge;
            _ads = new Dictionary<string, IAd>();
            _displayer = MediationManager.Instance.AdDisplayer;
        }

        public void Destroy() {
            foreach (var ad in _ads.Values) {
                ad.Destroy();
            }
            _ads.Clear();
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
            var ad = new GuardedBannerAd(new AdMobBannerAd(_bridge, this, adId, size));
            _ads.Add(adId, ad);
            return ad;
        }

        public IFullScreenAd CreateAppOpenAd(string adId) {
            return CreateFullScreenAd(kCreateAppOpenAd, adId,
                () => new AdMobAppOpenAd(_bridge, _displayer, this, adId));
        }

        public IFullScreenAd CreateInterstitialAd(string adId) {
            return CreateFullScreenAd(kCreateInterstitialAd, adId,
                () => new AdMobInterstitialAd(_bridge, _displayer, this, adId));
        }

        public IFullScreenAd CreateRewardedAd(string adId) {
            return CreateFullScreenAd(kCreateRewardedAd, adId,
                () => new AdMobRewardedAd(_bridge, _displayer, this, adId));
        }

        private IFullScreenAd CreateFullScreenAd(string handlerId, string adId, Func<IFullScreenAd> creator) {
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

        internal bool DestroyBannerAd(string adId) {
            return DestroyAd(kDestroyBannerAd, adId);
        }

        internal bool DestroyAppOpenAd(string adId) {
            return DestroyAd(kDestroyAppOpenAd, adId);
        }

        internal bool DestroyInterstitialAd(string adId) {
            return DestroyAd(kDestroyInterstitialAd, adId);
        }

        internal bool DestroyRewardedAd(string adId) {
            return DestroyAd(kDestroyRewardedAd, adId);
        }

        private bool DestroyAd(string handlerId, string adId) {
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