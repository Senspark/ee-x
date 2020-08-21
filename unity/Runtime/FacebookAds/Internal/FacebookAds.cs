using System;
using System.Collections.Generic;

using UnityEngine;
using UnityEngine.Assertions;

namespace EE.Internal {
    internal class FacebookAds : IFacebookAds {
        private const string kPrefix = "FacebookAdsBridge";
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
        private readonly Dictionary<string, IAdView> _bannerAds;
        private readonly Dictionary<string, IInterstitialAd> _interstitialAds;
        private readonly Dictionary<string, IRewardedAd> _rewardedAds;
        private readonly IAsyncHelper<bool> _interstitialAdDisplayer;
        private readonly IAsyncHelper<IRewardedAdResult> _rewardedAdDisplayer;

        public FacebookAds(IMessageBridge bridge) {
            _bridge = bridge;
            _bannerAds = new Dictionary<string, IAdView>();
            _interstitialAds = new Dictionary<string, IInterstitialAd>();
            _rewardedAds = new Dictionary<string, IRewardedAd>();
            _interstitialAdDisplayer = new AsyncHelper<bool>();
            _rewardedAdDisplayer = new AsyncHelper<IRewardedAdResult>();
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

        public IAdView CreateBannerAd(string adId, FacebookBannerAdSize adSize) {
            if (_bannerAds.TryGetValue(adId, out var result)) {
                return result;
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
            var ad = new GuardedAdView(new FacebookBannerAd(_bridge, this, adId, size));
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
            var ad = new GuardedInterstitialAd(
                new FacebookInterstitialAd(_bridge, _interstitialAdDisplayer, this, adId));
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
            var ad = new GuardedRewardedAd(new FacebookRewardedAd(_bridge, _rewardedAdDisplayer, this, adId));
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