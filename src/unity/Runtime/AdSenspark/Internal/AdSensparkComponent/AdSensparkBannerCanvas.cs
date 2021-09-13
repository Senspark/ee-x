using System;

using UnityEngine;
using UnityEngine.UI;

namespace EE.Internal {
    [RequireComponent(typeof(RectTransform))]
    internal class AdSensparkBannerCanvas : MonoBehaviour{
        private Image _image;
        private Action _cbClickAd;
        
        public void Initialize(Action cbClickAd, string resourceLocalPath) {
            _cbClickAd = cbClickAd;
            InitializeContainer();
            InitializeBanner(resourceLocalPath);
            ConfigBannerSize();
        }
        
        private void InitializeContainer() {
            var bgRectTransform = gameObject.GetComponent<RectTransform>();
            bgRectTransform.anchorMin = Vector2.zero;
            bgRectTransform.anchorMax = Vector2.one;
            bgRectTransform.offsetMin = Vector2.zero;
            bgRectTransform.offsetMax = Vector2.zero;
        }

        private void InitializeBanner(string resourceLocalPath) {
            var newObject = new GameObject{name = "banner"};
            _image = newObject.AddComponent<Image>();
            newObject.transform.SetParent(transform);
            newObject.transform.localScale = Vector3.one;
            
            var buttonBanner = newObject.AddComponent<Button>();
            buttonBanner.onClick.AddListener(OnButtonAdPressed);
            
            var sprite = Resources.Load<Sprite>(resourceLocalPath);
            if (!sprite) {
                Debug.LogWarning($"Ad senspark: không tìm thấy sprite tại đường dẫn {resourceLocalPath}");
                return;
            }
            _image.sprite = sprite;
        }

        /// <summary>
        /// Chỉnh sửa banner size thành 300x50 dpi
        /// </summary>
        private void ConfigBannerSize() {
            var heightPixel = Screen.dpi * 50 / 160;
            var widthPixel = Screen.dpi * 300 / 160;
            _image.rectTransform.sizeDelta = new Vector2(widthPixel, heightPixel);
        }

        public void SafeDestroy() {
            Destroy(gameObject);
        }

        public void SetVisible(bool display) {
            gameObject.SetActive(display);
        }

        private void OnButtonAdPressed() {
            _cbClickAd?.Invoke();
        }

        public void SetTexture(Texture2D texture2D) {
            var rect = new Rect(0, 0, texture2D.width, texture2D.height);
            var sprite = Sprite.Create(texture2D, rect, _image.rectTransform.pivot);
            _image.sprite = sprite;
        }
        
        public void SetAnchorBanner((float, float) valueTuple) {
            var rectImageBanner = _image.GetComponent<RectTransform>();
            rectImageBanner.pivot = new Vector2(valueTuple.Item1, valueTuple.Item2);
        }

        public void SetPositionBanner((float, float) valueTuple) {
            var rectImageBanner = _image.GetComponent<RectTransform>();
            rectImageBanner.position = new Vector2(valueTuple.Item1, valueTuple.Item2);
        }

        public void SetSizeBanner((float, float) valueTuple) {
            var rectImageBanner = _image.GetComponent<RectTransform>();
            rectImageBanner.sizeDelta = new Vector2(valueTuple.Item1, valueTuple.Item2);
        }
    }
}