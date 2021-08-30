using System;
using System.Threading.Tasks;

using UnityEngine;
using UnityEngine.Scripting;
using UnityEngine.UI;

namespace EE.Internal {
    internal class AdSensparkCanvas : MonoBehaviour {
        [SerializeField]
        private GameObject containerBanner, containerInterstitial, containerRewarded,
            containerRectBanner;
        [SerializeField]
        private Image imageBanner, imageInterstitial, imageRewarded, imageRectBanner;
        [SerializeField]
        private RectTransform rectImageBanner, rectImageRectBanner;

        private Action _onClickBanner, _onClickInterstitial, _onClickRewarded, _onClickRectBanner;
        private Action<AdResult> _onCloseInterstitial;
        
        private void Awake() {
            ServiceLocatorSimple.AddService(this);
            DontDestroyOnLoad(gameObject);
        }

        private void OnDestroy() {
            ServiceLocatorSimple.RemoveService(this);
        }

        #region Banner

        public void InitializeBanner(Action onClick, bool visible) {
            _onClickBanner = onClick;
            SetVisibleBanner(visible);
        }

        public void SetVisibleBanner(bool value) {
            gameObject.SetActive(value);
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
            SetVisibleInterstitial(false);
        }

        public void SetVisibleInterstitial(bool value) {
            gameObject.SetActive(value);
        }

        public void OnPromotionPressedInterstitial() {
            _onClickInterstitial?.Invoke();
            Application.OpenURL("https://play.google.com/store/apps/dev?id=7830868662152106484");
        }

        public void OnClosePressedInterstitial() {
            _onCloseInterstitial?.Invoke(AdResult.Completed);
            gameObject.SetActive(false);
        }

        #endregion

        public Task<bool> Initialize() {
            return Task.FromResult(true);
        }

        public void Destroy() {
            
        }
    }
}