﻿using System.Threading.Tasks;

using Jsonite;

using UnityEngine;

namespace EE.Internal {
    internal class AdSensparkImplEditor : IAdSenspark {
        public void Destroy() {
        }

        public Task<bool> Initialize(string url) {
            Debug.Log("AdSensparkImplEditor Initialize ");
            return Task.FromResult(true);
        }

        public IBannerAd CreateBannerAd(string adId, AdSensparkBannerAdSize adSize, JsonObject node) {
            Debug.Log("AdSensparkImplEditor CreateBannerAd " + adId);
            return new NullBannerAd();
        }

        public IFullScreenAd CreateAppOpenAd(string adId, JsonObject node) {
            Debug.Log("AdSensparkImplEditor CreateAppOpenAd " + adId);
            return new NullFullScreenAd();
        }

        public IFullScreenAd CreateInterstitialAd(string adId, JsonObject node) {
            Debug.Log("AdSensparkImplEditor CreateInterstitialAd " + adId);
            return new NullFullScreenAd();
        }

        public IFullScreenAd CreateRewardedInterstitialAd(string adId, JsonObject node) {
            Debug.Log("AdSensparkImplEditor CreateRewardedInterstitialAd " + adId);
            return new NullFullScreenAd();
        }

        public IFullScreenAd CreateRewardedAd(string adId, JsonObject node) {
            Debug.Log("AdSensparkImplEditor CreateRewardedAd " + adId);
            return new NullFullScreenAd();
        }
    }
}