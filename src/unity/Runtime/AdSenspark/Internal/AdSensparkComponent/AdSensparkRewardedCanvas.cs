using System;

using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Video;

namespace EE.Internal {
    [RequireComponent(typeof(RectTransform))]
    public class AdSensparkRewardedCanvas : MonoBehaviour {
        private Text _txtCountDown;
        private VideoPlayer _videoPlayer;
        private int _duration;
        private Action _cbClose, _cbClickAd;
        private bool _initialize = false;

        public void Initialize(Action cbClickAd, Action cbClose, string resourceLocalPath) {
            _cbClickAd = cbClickAd;
            _cbClose = cbClose;
            InitializeContainer();
            InitializeVideo(resourceLocalPath);
            InitializeTxtCountdown();
            _initialize = true;
        }

        public void SafeDestroy() {
            Destroy(gameObject);
        }

        private void InitializeContainer() {
            var bgRectTransform = gameObject.GetComponent<RectTransform>();
            bgRectTransform.anchorMin = Vector2.zero;
            bgRectTransform.anchorMax = Vector2.one;
            bgRectTransform.offsetMin = Vector2.zero;
            bgRectTransform.offsetMax = Vector2.zero;

            var image = gameObject.AddComponent<Image>();
            image.color = Color.black;

            var button = gameObject.AddComponent<Button>();
            button.transition = Selectable.Transition.None;
            button.onClick.AddListener(OnButtonAdPressed);
        }

        private void InitializeVideo(string resourceLocalPath) {
            var videoGameObject = new GameObject {name = "video"};
            
            var rawImage = videoGameObject.AddComponent<RawImage>();
            var renderTexture = new RenderTexture(768, 768, 16);
            renderTexture.Create();
            renderTexture.name = "renderTextureRewarded";
            rawImage.texture = renderTexture;
            rawImage.SetNativeSize();
            
            var videoClip = Resources.Load<VideoClip>(resourceLocalPath);
            _videoPlayer = videoGameObject.AddComponent<VideoPlayer>();
            _videoPlayer.targetTexture = renderTexture;
            _videoPlayer.clip = videoClip;
            videoGameObject.transform.SetParent(transform);
            videoGameObject.transform.localScale = Vector3.one;
            videoGameObject.transform.localPosition = Vector3.zero;
        }

        private void InitializeTxtCountdown() {
            var font = (Font)Resources.GetBuiltinResource(typeof(Font), "Arial.ttf");
            var countdownGameObject = new GameObject {name = "txtCountDown"};
            _txtCountDown = countdownGameObject.AddComponent<Text>();
            _txtCountDown.fontStyle = FontStyle.Bold;
            _txtCountDown.color = Color.white;
            _txtCountDown.fontSize = 50;
            _txtCountDown.font = font;
            _txtCountDown.alignment = TextAnchor.MiddleCenter;
            
            var rectTransform = countdownGameObject.GetComponent<RectTransform>();
            rectTransform.SetParent(transform);
            rectTransform.localScale = Vector3.one;
            rectTransform.anchorMax = Vector2.one;
            rectTransform.anchorMin = Vector2.one;
            rectTransform.anchoredPosition = new Vector2(-70, -70);

            var button = countdownGameObject.AddComponent<Button>();
            button.onClick.AddListener(OnButtonClosePressed);
        }

        public void SetVisible(bool display) {
            gameObject.SetActive(display);
        }

        private void OnButtonClosePressed() {
            if (_duration > 0)
                return;
            _cbClose?.Invoke();
            gameObject.SetActive(false);
        }

        private void OnButtonAdPressed() {
            _cbClickAd?.Invoke();
        }

        public void SetVideoClip(string fileName) {
            _videoPlayer.url = Application.persistentDataPath + "/" + fileName;
            if(!_videoPlayer.isPlaying)
                _videoPlayer.Play();
        }

        private void OnEnable() {
            if (!_initialize)
                return;
            _duration = 30;
            _videoPlayer.Play();
            InvokeRepeating(nameof(CountDown), 1f, 1f);
            UpdateUI();
        }

        private void CountDown() {
            _duration -= 1;
            if(_duration <= 0) {
                UpdateUI();
                CancelInvoke(nameof(CountDown));
            } else {
                UpdateUI();
            }
        }

        private void UpdateUI() {
            _txtCountDown.text = _duration > 0 ? _duration.ToString() : "X";
        }
    }
}