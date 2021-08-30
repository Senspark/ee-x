using System;
using System.Threading.Tasks;

using UnityEngine;
using UnityEngine.Scripting;
using UnityEngine.UI;

namespace EE.Internal {
    internal class AdSensparkCanvas : MonoBehaviour {
        [SerializeField]
        private GameObject containerBanner, containerInterstitial, containerRewarded,
            containerRectBanner, containerAppOpen, containerRewardedInterstitial;
        [SerializeField]
        private Image imageBanner, imageInterstitial, imageRewarded, imageRectBanner;
        [SerializeField]
        private RectTransform rectImageBanner, rectImageRectBanner;

        private Action _onClickBanner, _onClickInterstitial, _onClickRewarded, _onClickRectBanner;
        private Action<AdResult> _onCloseInterstitial, _onCloseRewarded;
        
        private void Awake() {
            ServiceLocatorSimple.AddService(this);
            DontDestroyOnLoad(gameObject);

            containerBanner.SetActive(false);
            containerInterstitial.SetActive(false);
            containerRewarded.SetActive(false);
            containerRectBanner.SetActive(false);
            containerAppOpen.SetActive(false);
            containerRewardedInterstitial.SetActive(false);
        }

        private void OnDestroy() {
            ServiceLocatorSimple.RemoveService(this);
        }

        public void SetAdVisible(AdFormat adFormat, bool display) {
            switch (adFormat) {
                case AdFormat.Banner:
                    containerBanner.SetActive(display);
                    break;

                case AdFormat.Rectangle:
                    containerRectBanner.SetActive(display);
                    break;

                case AdFormat.AppOpen:
                    containerAppOpen.SetActive(display);
                    break;

                case AdFormat.Interstitial:
                    containerInterstitial.SetActive(display);
                    break;

                case AdFormat.RewardedInterstitial:
                    containerRewardedInterstitial.SetActive(display);
                    break;

                case AdFormat.Rewarded:
                    containerRewarded.SetActive(display);
                    break;

                case AdFormat.Null:
                    break;

                default:
                    throw new ArgumentOutOfRangeException(nameof(adFormat), adFormat, null);
            }
        }

        #region Banner

        public void InitializeBanner(Action onClick, bool display) {
            _onClickBanner = onClick;
            SetAdVisible(AdFormat.Banner, display);
        }

        public void SetAnchorBanner((float, float) valueTuple) {
            rectImageBanner.pivot = new Vector2(valueTuple.Item1, valueTuple.Item2);
        }

        public void SetPositionBanner((float, float) valueTuple) {
            rectImageBanner.position = new Vector2(valueTuple.Item1, valueTuple.Item2);
        }

        public void SetSizeBanner((float, float) valueTuple) {
            rectImageBanner.sizeDelta = new Vector2(valueTuple.Item1, valueTuple.Item2);
        }

        public void OnPromotionPressedBanner() {
            _onClickBanner?.Invoke();
            Application.OpenURL("https://play.google.com/store/apps/dev?id=7830868662152106484");
        }

        #endregion

        #region Interstitial

        public void InitializeInterstitial(Action onClick, Action<AdResult> onClose) {
            _onClickInterstitial = onClick;
            _onCloseInterstitial = onClose;
        }

        public void OnPromotionPressedInterstitial() {
            _onClickInterstitial?.Invoke();
            Application.OpenURL("https://play.google.com/store/apps/dev?id=7830868662152106484");
        }

        public void OnClosePressedInterstitial() {
            _onCloseInterstitial?.Invoke(AdResult.Completed);
            SetAdVisible(AdFormat.Interstitial, false);
        }

        #endregion
        
        #region Rewarded

        public void InitializeRewarded(Action onClick, Action<AdResult> onClose) {
            _onClickRewarded = onClick;
            _onCloseRewarded = onClose;
        }

        public void OnPromotionPressedRewarded() {
            _onClickRewarded?.Invoke();
            Application.OpenURL("https://play.google.com/store/apps/dev?id=7830868662152106484");
        }

        public void OnClosePressedRewarded() {
            _onCloseRewarded?.Invoke(AdResult.Completed);
            SetAdVisible(AdFormat.Rewarded, false);
        }

        #endregion
    }
}