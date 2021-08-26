using System;

using UnityEngine;
using UnityEngine.UI;

namespace EE.Internal {
    internal class AdSensparkBannerCanvas : MonoBehaviour {
        [SerializeField]
        private Image image;
        [SerializeField]
        private RectTransform rectImage;
        private Action _onClick;
        
        private void Awake() {
            DontDestroyOnLoad(gameObject);
        }

        public void Initialize(Action onClick, bool visible) {
            _onClick = onClick;
            SetVisible(visible);
        }

        public void SetVisible(bool value) {
            gameObject.SetActive(value);
        }

        public void SetAnchor((float, float) valueTuple) {
            rectImage.pivot = new Vector2(valueTuple.Item1, valueTuple.Item2);
        }

        public void SetPosition((float, float) valueTuple) {
            rectImage.position = new Vector2(valueTuple.Item1, valueTuple.Item2);
        }

        public void SetSize((float, float) valueTuple) {
            rectImage.sizeDelta = new Vector2(valueTuple.Item1, valueTuple.Item2);
        }

        public void OnPromotionPressed() {
            _onClick?.Invoke();
            Application.OpenURL("https://play.google.com/store/apps/dev?id=7830868662152106484");
        }
    }
}