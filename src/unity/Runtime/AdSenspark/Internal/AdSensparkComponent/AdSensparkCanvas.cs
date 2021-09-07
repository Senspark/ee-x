using System;
using System.Threading.Tasks;

using UnityEngine;
using UnityEngine.Scripting;
using UnityEngine.UI;
using UnityEngine.Video;

namespace EE.Internal {
    internal class AdSensparkCanvas : MonoBehaviour {
        [SerializeField]
        private GameObject containerBanner, containerInterstitial, containerRewarded,
            containerRectBanner, containerAppOpen, containerRewardedInterstitial;
        [SerializeField]
        private Image imageBanner, imageInterstitial;
        [SerializeField]
        private VideoPlayer rewardedVideoPlayer;
        [SerializeField]
        private RectTransform rectImageBanner;
        

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

        // private void Start() {
        //     Utils.NoAwait(TestLoad);
        // }
        //
        // private async Task TestLoad() {
        //     // var link = "https://drive.google.com/uc?export=download&id=1nItU1hDk8NGCve4p8vpzrRDyulBiNqHy"; // interstitial
        //     var link = "https://drive.google.com/uc?export=download&id=1MsFwDtHORm2ARL5-F5xE7OB--ARwKp1R"; // video.
        //     var fileName = "rewarded.mov";
        //     var data = await Downloader.Load(fileName, link);
        //     // SetImageTexture(AdFormat.Rewarded, data);
        //     if (data.Length > 0) {
        //         SetVideoClip(AdFormat.Rewarded, fileName);
        //     }
        // }

        private void OnDestroy() {
            ServiceLocatorSimple.RemoveService(this);
        }

        /// <summary>
        /// Set hình cho banner, interstitial.
        /// </summary>
        /// <param name="adFormat"></param>
        /// <param name="data"></param>
        public void SetImageTexture(AdFormat adFormat, Byte[] data) {
            Texture2D texture2D = new Texture2D(2, 2);
            texture2D.LoadImage(data);
            SetImageTexture(adFormat, texture2D);
        }

        /// <summary>
        /// Set hình cho banner, interstitial.
        /// </summary>
        /// <param name="adFormat"></param>
        /// <param name="data"></param>
        public void SetImageTexture(AdFormat adFormat, Texture2D texture2D) {
            switch (adFormat) {
                case AdFormat.Banner: {
                    var rect = new Rect(0, 0, texture2D.width, texture2D.height);
                    var sprite = Sprite.Create(texture2D, rect, imageBanner.rectTransform.pivot);
                    imageBanner.sprite = sprite;
                }
                    break;

                case AdFormat.Rectangle:
                    break;

                case AdFormat.AppOpen:
                    break;

                case AdFormat.Interstitial: {
                    var rect = new Rect(0, 0, texture2D.width, texture2D.height);
                    var sprite = Sprite.Create(texture2D, rect, imageInterstitial.rectTransform.pivot);
                    imageInterstitial.sprite = sprite;
                }
                    break;

                default:
                    throw new ArgumentOutOfRangeException(nameof(adFormat), adFormat, null);
            }
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

        /// <summary>
        /// Set video cho rewarded.
        /// https://forum.unity.com/threads/byte-to-audioclip.911723/
        /// </summary>
        /// <param name="adFormat"></param>
        /// <param name="data"></param>
        public void SetVideoClip(AdFormat adFormat, string fileName) {
            switch (adFormat) {
                case AdFormat.RewardedInterstitial:
                    break;

                case AdFormat.Rewarded: {
                    rewardedVideoPlayer.url = Application.persistentDataPath + "/" + fileName;
                    if(!rewardedVideoPlayer.isPlaying)
                        rewardedVideoPlayer.Play();
                }
                    break;

                default:
                    throw new ArgumentOutOfRangeException(nameof(adFormat), adFormat, null);
            }
        }

        #endregion
    }
}