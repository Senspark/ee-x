using System;
using System.Threading.Tasks;

using UnityEngine;
using UnityEngine.UI;

namespace EE.Internal {
    public class AdSensparkRewarded : MonoBehaviour {
        [SerializeField]
        private GameObject btnClose;
        [SerializeField]
        private Text txtCountDown;
        [SerializeField]
        private Image imgLoading;

        private int _duration;

        private void OnEnable() {
            _duration = 30;
            btnClose.SetActive(false);
            imgLoading.gameObject.SetActive(true);
            InvokeRepeating(nameof(CountDown), 1f, 1f);
            UpdateUI();
        }

        private void CountDown() {
            _duration -= 1;
            if(_duration <= 0) {
                btnClose.SetActive(true);
                imgLoading.gameObject.SetActive(false);
                CancelInvoke(nameof(CountDown));
            } else {
                UpdateUI();
            }
        }

        private void UpdateUI() {
            txtCountDown.text = _duration.ToString();
            var percent = _duration / 30.0f;
            imgLoading.fillAmount = percent;
        }
    }
}