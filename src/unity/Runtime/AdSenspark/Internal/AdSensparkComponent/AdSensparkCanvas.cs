using System;

using UnityEngine;
using UnityEngine.UI;

namespace EE.Internal {
    internal class AdSensparkCanvas : MonoBehaviour {
        private AdSensparkBannerCanvas _adSensparkBannerCanvas;
        private AdSensparkInterstitialCanvas _adSensparkInterstitialCanvas;
        private AdSensparkRewardedCanvas _adSensparkRewardedCanvas;

        private Action _onClickBanner, 
            _onClickInterstitial, 
            _onClickRewarded;
        private Action<AdResult> _onCloseInterstitial, 
            _onCloseRewarded;

        private RectTransform _rectTransform;
        
        private void Awake() {
            gameObject.AddComponent<GraphicRaycaster>();
            _rectTransform = GetComponent<RectTransform>();
            
            var canvas = gameObject.GetComponent<Canvas>();
            canvas.renderMode = RenderMode.ScreenSpaceOverlay;
            canvas.sortingOrder = 500;
            
            var canvasScaler = gameObject.AddComponent<CanvasScaler>();
            var isHorizontal = Screen.width > Screen.height;
            canvasScaler.uiScaleMode = CanvasScaler.ScaleMode.ScaleWithScreenSize;
            canvasScaler.screenMatchMode = CanvasScaler.ScreenMatchMode.MatchWidthOrHeight;
            canvasScaler.matchWidthOrHeight = isHorizontal ? 1 : 0;
            canvasScaler.referenceResolution = new Vector2(768, 768);
        }

        #region CommonFunc

        /// <summary>
        /// Set video cho rewarded.
        /// https://forum.unity.com/threads/byte-to-audioclip.911723/
        /// </summary>
        /// <param name="adFormat"></param>
        /// <param name="data"></param>
        private void SetVideoClip(AdFormat adFormat, string fileName) {
            switch (adFormat) {
                case AdFormat.RewardedInterstitial:
                    break;

                case AdFormat.Rewarded: {
                    _adSensparkRewardedCanvas.SetVideoClip(fileName);
                }
                    break;

                default:
                    throw new ArgumentOutOfRangeException(nameof(adFormat), adFormat, null);
            }
        }

        private void OnDestroy() {
            ServiceLocatorSimple.RemoveService(this);
        }

        public void SetAdData(AdFormat adFormat, AdSensparkResourcePack dataPack) {
            if (dataPack.IsNull())
                return;
            
            switch (adFormat) {
                case AdFormat.Banner:
                case AdFormat.Interstitial:
                    SetImageTexture(adFormat, dataPack.data);
                    break;

                case AdFormat.Rewarded:
                    SetVideoClip(adFormat, dataPack.fileName);
                    break;
            }
        }

        /// <summary>
        /// Set hình cho banner, interstitial.
        /// </summary>
        /// <param name="adFormat"></param>
        /// <param name="data"></param>
        private void SetImageTexture(AdFormat adFormat, Byte[] data) {
            Texture2D texture2D = new Texture2D(2, 2);
            texture2D.LoadImage(data);
            SetImageTexture(adFormat, texture2D);
        }

        /// <summary>
        /// Set hình cho banner, interstitial.
        /// </summary>
        /// <param name="adFormat"></param>
        /// <param name="texture2D"></param>
        private void SetImageTexture(AdFormat adFormat, Texture2D texture2D) {
            switch (adFormat) {
                case AdFormat.Banner: {
                    _adSensparkBannerCanvas.SetTexture(texture2D);
                }
                    break;

                case AdFormat.Interstitial: {
                    _adSensparkInterstitialCanvas.SetTexture(texture2D);
                }
                    break;

                default:
                    throw new ArgumentOutOfRangeException(nameof(adFormat), adFormat, null);
            }
        }

        public void SetAdVisible(AdFormat adFormat, bool display) {
            switch (adFormat) {
                case AdFormat.Banner:
                    _adSensparkBannerCanvas.SetVisible(display);
                    break;

                case AdFormat.Interstitial:
                    _adSensparkInterstitialCanvas.SetVisible(display);
                    break;

                case AdFormat.Rewarded:
                    _adSensparkRewardedCanvas.SetVisible(display);
                    break;

                case AdFormat.Null:
                    break;

                default:
                    throw new ArgumentOutOfRangeException(nameof(adFormat), adFormat, null);
            }
        }

        #endregion CommonFuc

        #region Banner

        public void InitializeBanner(Action onClick, bool display, string resourceLocalPath) {
            _onClickBanner = onClick;
            CreateBanner(resourceLocalPath);
            SetAdVisible(AdFormat.Banner, display);
        }

        private void CreateBanner(string spriteLocalPath) {
            if (_adSensparkBannerCanvas) {
                _adSensparkBannerCanvas.SafeDestroy();
            }
            
            var newObject = new GameObject {name = "ContainerBanner"};
            _adSensparkBannerCanvas = newObject.AddComponent<AdSensparkBannerCanvas>();
            newObject.transform.SetParent(_rectTransform);
            newObject.transform.localScale = Vector3.one;
            newObject.SetActive(false);
            _adSensparkBannerCanvas.Initialize(OnPromotionPressedBanner, spriteLocalPath);
        }

        public void SetAnchorBanner((float, float) valueTuple) {
            _adSensparkBannerCanvas.SetAnchorBanner(valueTuple);
        }

        public void SetPositionBanner((float, float) valueTuple) {
            _adSensparkBannerCanvas.SetPositionBanner(valueTuple);
        }

        public void SetSizeBanner((float, float) valueTuple) {
            _adSensparkBannerCanvas.SetSizeBanner(valueTuple);
        }

        public void OnPromotionPressedBanner() {
            _onClickBanner?.Invoke();
        }

        #endregion

        #region Interstitial

        public void InitializeInterstitial(Action onClick, Action<AdResult> onClose, string resourceLocalPath) {
            _onClickInterstitial = onClick;
            _onCloseInterstitial = onClose;
            CreateInterstitial(resourceLocalPath);
        }

        private void CreateInterstitial(string spriteLocalPath) {
            if (_adSensparkInterstitialCanvas) {
                _adSensparkInterstitialCanvas.SafeDestroy();
            }
            var newObject = new GameObject {name = "ContainerInterstitial"};
            _adSensparkInterstitialCanvas = newObject.AddComponent<AdSensparkInterstitialCanvas>();
            newObject.transform.SetParent(_rectTransform);
            newObject.transform.localScale = Vector3.one;
            newObject.SetActive(false);
            _adSensparkInterstitialCanvas.Initialize(OnPromotionPressedInterstitial, OnClosePressedInterstitial, spriteLocalPath);
        }

        public void OnPromotionPressedInterstitial() {
            _onClickInterstitial?.Invoke();
        }

        public void OnClosePressedInterstitial() {
            _onCloseInterstitial?.Invoke(AdResult.Completed);
            SetAdVisible(AdFormat.Interstitial, false);
        }

        #endregion
        
        #region Rewarded

        public void InitializeRewarded(Action onClick, Action<AdResult> onClose, string resourceLocalPath) {
            _onClickRewarded = onClick;
            _onCloseRewarded = onClose;
            
            CreateRewarded(resourceLocalPath);
        }

        private void CreateRewarded(string videoLocalPath) {
            if(_adSensparkRewardedCanvas) {
                _adSensparkRewardedCanvas.SafeDestroy();
            }
            var newObject = new GameObject {name = "ContainerRewarded"};
            _adSensparkRewardedCanvas = newObject.AddComponent<AdSensparkRewardedCanvas>();         
            newObject.transform.SetParent(transform);
            newObject.transform.localScale = Vector3.one;
            newObject.SetActive(false);
            _adSensparkRewardedCanvas.Initialize(OnPromotionPressedRewarded, OnClosePressedRewarded, videoLocalPath);
        }

        public void OnPromotionPressedRewarded() {
            _onClickRewarded?.Invoke();
        }

        public void OnClosePressedRewarded() {
            _onCloseRewarded?.Invoke(AdResult.Completed);
            SetAdVisible(AdFormat.Rewarded, false);
        }

        #endregion
    }
}