using UnityEngine;
using System;

namespace EE.Internal {
    [Serializable]
    internal class AdSensparkJsonData {
        public string[] bannerImageUrl;
        public string[] bannerPromotionUrl;
        public string[] interstitialImageUrl;
        public string[] interstitialPromotionUrl;
        public string[] rewardedVideoUrl;
        public string[] rewardedPromotionUrl;

        public string GetDataUrl(AdFormat adFormat, int index) {
            switch (adFormat) {
                case AdFormat.Banner: {
                    var rollIndex = index % bannerImageUrl.Length;
                    return bannerImageUrl[rollIndex];
                }
                
                case AdFormat.Interstitial: {
                    var rollIndex = index % interstitialImageUrl.Length;
                    return interstitialImageUrl[rollIndex];
                }

                case AdFormat.Rewarded: {
                    var rollIndex = index % rewardedVideoUrl.Length;
                    return rewardedVideoUrl[rollIndex];
                }

                default: {
                    Debug.LogWarning("Ad senspark json data: chưa hỗ trợ ad loại này.");
                    return "";
                }
            }
        }

        public string GetPromotionUrl(AdFormat adFormat, int index) {
            switch (adFormat) {
                case AdFormat.Banner: {
                    var rollIndex = index % bannerPromotionUrl.Length;
                    return bannerPromotionUrl[rollIndex];
                }
                
                case AdFormat.Interstitial: {
                    var rollIndex = index % interstitialPromotionUrl.Length;
                    return interstitialPromotionUrl[rollIndex];
                }

                case AdFormat.Rewarded: {
                    var rollIndex = index % rewardedPromotionUrl.Length;
                    return rewardedPromotionUrl[rollIndex];
                }

                default:{
                    Debug.LogWarning("Ad senspark json data: chưa hỗ trợ ad loại này.");
                    return "";
                }
            }
        }
    }
}