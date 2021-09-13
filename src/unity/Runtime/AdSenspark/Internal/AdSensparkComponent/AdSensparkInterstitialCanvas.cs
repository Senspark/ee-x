using System;

using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Video;

namespace EE.Internal {
    [RequireComponent(typeof(RectTransform))]
    internal class AdSensparkInterstitialCanvas : MonoBehaviour {
        private Image _image;
        private Action _cbClose, _cbClickAd;
        
        public void Initialize(Action cbClickAd, Action cbClose, string resourceLocalPath) {
            _cbClickAd = cbClickAd;
            _cbClose = cbClose;
            InitializeContainer();
            InitializeInterstitial(resourceLocalPath);
            InitializeButtonClose();
        }

        private void InitializeContainer() {
            var bgRectTransform = gameObject.GetComponent<RectTransform>();
            bgRectTransform.anchorMin = Vector2.zero;
            bgRectTransform.anchorMax = Vector2.one;
            bgRectTransform.offsetMin = Vector2.zero;
            bgRectTransform.offsetMax = Vector2.zero;

            var image = gameObject.AddComponent<Image>();
            image.color = Color.black; 
        }

        private void InitializeInterstitial(string resourceLocalPath) {
            var newObject = new GameObject{name = "interstitial"};
            _image = newObject.AddComponent<Image>();
            var rectTransform = newObject.GetComponent<RectTransform>();
            rectTransform.SetParent(transform);
            rectTransform.localScale = Vector3.one;
            rectTransform.localPosition = Vector3.zero;
            
            var button = newObject.AddComponent<Button>();
            button.onClick.AddListener(OnButtonAdPressed);
            
            var sprite = Resources.Load<Sprite>(resourceLocalPath);
            if (!sprite) {
                Debug.LogWarning($"Ad senspark: không tìm thấy sprite tại đường dẫn {resourceLocalPath}");
                return;
            }
            _image.sprite = sprite;
            _image.SetNativeSize();
        }
        
        private void InitializeButtonClose() {
            var font = (Font)Resources.GetBuiltinResource(typeof(Font), "Arial.ttf");
            var countdownGameObject = new GameObject {name = "ButtonClose"};
            var text = countdownGameObject.AddComponent<Text>();
            text.fontStyle = FontStyle.Bold;
            text.color = Color.red;
            text.fontSize = 50;
            text.font = font;
            text.alignment = TextAnchor.MiddleCenter;
            text.text = "X";
            
            var rectTransform = countdownGameObject.GetComponent<RectTransform>();
            rectTransform.SetParent(transform);
            rectTransform.localScale = Vector3.one;
            rectTransform.anchorMax = Vector2.one;
            rectTransform.anchorMin = Vector2.one;
            rectTransform.anchoredPosition = new Vector2(-70, -70);

            var button = countdownGameObject.AddComponent<Button>();
            button.onClick.AddListener(OnButtonClosePressed);
        }
        
        public void SafeDestroy() {
            Destroy(gameObject);
        }

        public void SetVisible(bool display) {
            gameObject.SetActive(display);
        }

        private void OnButtonClosePressed() {
            _cbClose?.Invoke();
            gameObject.SetActive(false);    
        }

        private void OnButtonAdPressed() {
            _cbClickAd?.Invoke();
        }

        public void SetTexture(Texture2D texture2D) {
            var rect = new Rect(0, 0, texture2D.width, texture2D.height);
            var sprite = Sprite.Create(texture2D, rect, _image.rectTransform.pivot);
            _image.sprite = sprite;
        }
    }
}