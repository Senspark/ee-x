using System;

using UnityEngine;
using UnityEngine.UI;
namespace EE.Internal {
    internal class AdSensparkInterstitialCanvas : MonoBehaviour {
        [SerializeField]
        private Image image;
        [SerializeField]
        private RectTransform rectImage;
        private Action _onClick;
        private Action<AdResult> _onClose;
        
        private void Awake() {
            DontDestroyOnLoad(gameObject);
        }

        public void Initialize(Action onClick, Action<AdResult> onClose) {
            _onClick = onClick;
            _onClose = onClose;
            SetVisible(false);
        }

        public void SetVisible(bool value) {
            gameObject.SetActive(value);
        }

        public void OnPromotionPressed() {
            _onClick?.Invoke();
            Application.OpenURL("https://play.google.com/store/apps/dev?id=7830868662152106484");
        }

        public void OnClosePressed() {
            _onClose?.Invoke(AdResult.Completed);
            gameObject.SetActive(false);
        }
    }
}