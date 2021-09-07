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
    }
}