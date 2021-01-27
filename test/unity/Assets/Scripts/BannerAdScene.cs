using System.Collections.Generic;

using DG.Tweening;

using UnityEngine;

namespace EETest {
    public class BannerAdScene : MonoBehaviour {
        private readonly List<Tween> _tweens = new List<Tween>();

        public EE.IAdsManager AdsManager { get; set; }

        public void Execute() {
            AdsManager.IsBannerAdVisible = true;
            var (width, height) = EE.Platform.GetViewSize();
            _tweens.Add(DOTween.Sequence()
                .SetDelay(2.0f)
                .AppendInterval(8.0f)
                .AppendCallback(() => {
                    Debug.Log($"Move to top-left");
                    AdsManager.BannerAdAnchor = (0, 1);
                    AdsManager.BannerAdPosition = (0, height);
                }));
            _tweens.Add(DOTween.Sequence()
                .SetDelay(4.0f)
                .AppendInterval(8.0f)
                .AppendCallback(() => {
                    Debug.Log($"Move to top-right");
                    AdsManager.BannerAdAnchor = (1, 1);
                    AdsManager.BannerAdPosition = (width, height);
                }));
            _tweens.Add(DOTween.Sequence()
                .SetDelay(6.0f)
                .AppendInterval(8.0f)
                .AppendCallback(() => {
                    Debug.Log($"Move to bottom-right");
                    AdsManager.BannerAdAnchor = (1, 0);
                    AdsManager.BannerAdPosition = (width, 0);
                }));
            _tweens.Add(DOTween.Sequence()
                .SetDelay(8.0f)
                .AppendInterval(8.0f)
                .AppendCallback(() => {
                    Debug.Log($"Move to bottom-left");
                    AdsManager.BannerAdAnchor = (0, 0);
                    AdsManager.BannerAdPosition = (0, 0);
                }));
        }

        private void OnDestroy() {
            foreach (var tween in _tweens) {
                tween.Kill();
            }
            _tweens.Clear();
        }
    }
}