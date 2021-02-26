using UnityEngine;
using UnityEngine.UI;

namespace EETest {
    public class FullScreenAdScene : MonoBehaviour {
        [SerializeField]
        private Text _resultText;

        public EE.IAdsManager AdsManager { get; set; }

        private void UpdateResult(EE.AdResult result) {
            switch (result) {
                case EE.AdResult.NotInitialized:
                    _resultText.text = "Not Initialized";
                    break;
                case EE.AdResult.NotConfigured:
                    _resultText.text = "Not Configured";
                    break;
                case EE.AdResult.Capped:
                    _resultText.text = "Capped";
                    break;
                case EE.AdResult.NoInternet:
                    _resultText.text = "No Internet";
                    break;
                case EE.AdResult.NotLoaded:
                    _resultText.text = "Not Loaded";
                    break;
                case EE.AdResult.Failed:
                    _resultText.text = "Failed";
                    break;
                case EE.AdResult.Canceled:
                    _resultText.text = "Canceled";
                    break;
                case EE.AdResult.Completed:
                    _resultText.text = "Completed";
                    break;
            }
        }

        public void OnShowAppOpenAdButtonPressed() {
            EE.Utils.NoAwait(async () => {
                _resultText.text = "---";
                var result = await AdsManager.AppOpenAd.Show();
                UpdateResult(result);
            });
        }

        public void OnShowInterstitialAdButtonPressed() {
            EE.Utils.NoAwait(async () => {
                _resultText.text = "---";
                var result = await AdsManager.InterstitialAd.Show();
                UpdateResult(result);
            });
        }

        public void OnShowRewardedInterstitialAdButtonPressed() {
            EE.Utils.NoAwait(async () => {
                _resultText.text = "---";
                var result = await AdsManager.RewardedInterstitialAd.Show();
                UpdateResult(result);
            });
        }

        public void OnShowRewardedAdButtonPressed() {
            EE.Utils.NoAwait(async () => {
                _resultText.text = "---";
                var result = await AdsManager.RewardedAd.Show();
                UpdateResult(result);
            });
        }
    }
}