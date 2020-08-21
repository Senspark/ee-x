using System;
using System.Collections.Generic;

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
        private const string kCreateInterstitialAd = kPrefix + "CreateInterstitialAd";
        private const string kDestroyInterstitialAd = kPrefix + "DestroyInterstitialAd";
        private const string kCreateRewardedAd = kPrefix + "CreateRewardedAd";
        private const string kDestroyRewardedAd = kPrefix + "DestroyRewardedAd";

        private readonly IMessageBridge _bridge;
        private readonly Dictionary<string, IAdView> _bannerAds;
        private readonly Dictionary<string, IInterstitialAd> _interstitialAds;
        private readonly Dictionary<string, IRewardedAd> _rewardedAds;
        private readonly IAsyncHelper<bool> _interstitialAdDisplayer;
        private readonly IAsyncHelper<IRewardedAdResult> _rewardedAdDisplayer;

        public AdMob(IMessageBridge bridge) {
            _bridge = bridge;
            _bannerAds = new Dictionary<string, IAdView>();
            _interstitialAds = new Dictionary<string, IInterstitialAd>();
            _rewardedAds = new Dictionary<string, IRewardedAd>();
            _interstitialAdDisplayer = MediationManager.Instance.InterstitialAdDisplayer;
            _rewardedAdDisplayer = MediationManager.Instance.RewardedAdDisplayer;
        }

        public void Destroy() {
            foreach (var ad in _bannerAds.Values) {
                ad.Destroy();
            }
            foreach (var ad in _interstitialAds.Values) {
                ad.Destroy();
            }
            foreach (var ad in _rewardedAds.Values) {
                ad.Destroy();
            }
            _bannerAds.Clear();
            _interstitialAds.Clear();
            _rewardedAds.Clear();
        }

        public void Initialize() {
            _bridge.Call(kInitialize);
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

        public IAdView CreateBannerAd(string adId, AdMobBannerAdSize adSize) {
            if (_bannerAds.TryGetValue(adId, out var result)) {
                return result;
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
            var ad = new GuardedAdView(new AdMobBannerAd(_bridge, this, adId, size));
            _bannerAds.Add(adId, ad);
            return ad;
        }

        internal bool DestroyBannerAd(string adId) {
            if (!_bannerAds.ContainsKey(adId)) {
                return false;
            }
            var response = _bridge.Call(kDestroyBannerAd, adId);
            if (!Utils.ToBool(response)) {
                Assert.IsTrue(false);
                return false;
            }
            _bannerAds.Remove(adId);
            return true;
        }

        public IInterstitialAd CreateInterstitialAd(string adId) {
            if (_interstitialAds.TryGetValue(adId, out var result)) {
                return result;
            }
            var response = _bridge.Call(kCreateInterstitialAd, adId);
            if (!Utils.ToBool(response)) {
                Assert.IsTrue(false);
                return null;
            }
            var ad = new GuardedInterstitialAd(new AdMobInterstitialAd(_bridge, _interstitialAdDisplayer, this, adId));
            _interstitialAds.Add(adId, ad);
            return ad;
        }

        internal bool DestroyInterstitialAd(string adId) {
            if (!_interstitialAds.ContainsKey(adId)) {
                return false;
            }
            var response = _bridge.Call(kDestroyInterstitialAd, adId);
            if (!Utils.ToBool(response)) {
                Assert.IsTrue(false);
                return false;
            }
            _interstitialAds.Remove(adId);
            return true;
        }

        public IRewardedAd CreateRewardedAd(string adId) {
            if (_rewardedAds.TryGetValue(adId, out var result)) {
                return result;
            }
            var response = _bridge.Call(kCreateRewardedAd, adId);
            if (!Utils.ToBool(response)) {
                Assert.IsTrue(false);
                return null;
            }
            var ad = new GuardedRewardedAd(new AdMobRewardedAd(_bridge, _rewardedAdDisplayer, this, adId));
            _rewardedAds.Add(adId, ad);
            return ad;
        }

        internal bool DestroyRewardedAd(string adId) {
            if (!_rewardedAds.ContainsKey(adId)) {
                return false;
            }
            var response = _bridge.Call(kDestroyRewardedAd, adId);
            if (!Utils.ToBool(response)) {
                Assert.IsTrue(false);
                return false;
            }
            _rewardedAds.Remove(adId);
            return true;
        }
    }
}