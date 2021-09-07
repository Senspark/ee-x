using UnityEngine;
using System;

namespace EE.Internal {
    [Serializable]
    internal class AdSensparkJsonData {
        public string[] bannerImageUrl;
        public string[] bannerPromotionUrlAndroid;
        public string[] bannerPromotionUrliOS;
        public string[] interstitialImageUrl;
        public string[] interstitialPromotionUrlAndroid;
        public string[] interstitialPromotionUrliOS;
        public string[] rewardedVideoUrl;
        public string[] rewardedPromotionUrlAndroid;
        public string[] rewardedPromotionUrliOS;

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
            string[] strArray;
            switch (adFormat) {
                case AdFormat.Banner: {
                    strArray = Application.platform == RuntimePlatform.Android ? 
                        bannerPromotionUrlAndroid : bannerPromotionUrliOS;
                    break;
                }
                
                case AdFormat.Interstitial: {
                    strArray = Application.platform == RuntimePlatform.Android?
                        interstitialPromotionUrlAndroid : interstitialPromotionUrliOS;
                    break;
                }

                case AdFormat.Rewarded: {
                    strArray = Application.platform == RuntimePlatform.Android?
                        rewardedPromotionUrlAndroid : rewardedPromotionUrliOS;
                    break;
                }

                default:{
                    Debug.LogWarning("Ad senspark json data: chưa hỗ trợ ad loại này.");
                    return "";
                }
            }

            var rollIndex = index % strArray.Length;
            return strArray[rollIndex];
        }

        /// <summary>
        /// Lấy số lượng ad hiện có.
        /// </summary>
        /// <param name="adFormat"></param>
        /// <returns></returns>
        public int GetAdAmount(AdFormat adFormat) {
            switch (adFormat) {
                case AdFormat.Banner:
                    return bannerImageUrl.Length;
                
                case AdFormat.Interstitial:
                    return interstitialImageUrl.Length;

                case AdFormat.Rewarded:
                    return rewardedVideoUrl.Length;

                default:{
                    Debug.LogWarning("Ad senspark json data: chưa hỗ trợ ad loại này.");
                    return 0;
                }
            }
        }
    }
}