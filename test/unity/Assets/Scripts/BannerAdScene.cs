using System.Collections.Generic;

using DG.Tweening;

using UnityEngine;

namespace EETest {
    public class BannerAdScene : MonoBehaviour {
        private readonly List<Tween> _tweens = new List<Tween>();

        public EE.IAdsManager AdsManager { get; set; }

        public void Execute() {
            AdsManager.BannerAd.IsVisible = true;
            var (width, height) = EE.Platform.GetViewSize();
            _tweens.Add(DOTween.Sequence()
                .SetDelay(2.0f)
                .AppendInterval(8.0f)
                .AppendCallback(() => {
                    Debug.Log($"Move to top-left");
                    AdsManager.BannerAd.Anchor = (0, 1);
                    AdsManager.BannerAd.Position = (0, height);
                })
                .SetLoops(-1));
            _tweens.Add(DOTween.Sequence()
                .SetDelay(4.0f)
                .AppendInterval(8.0f)
                .AppendCallback(() => {
                    Debug.Log($"Move to top-right");
                    AdsManager.BannerAd.Anchor = (1, 1);
                    AdsManager.BannerAd.Position = (width, height);
                })
                .SetLoops(-1));
            _tweens.Add(DOTween.Sequence()
                .SetDelay(6.0f)
                .AppendInterval(8.0f)
                .AppendCallback(() => {
                    Debug.Log($"Move to bottom-right");
                    AdsManager.BannerAd.Anchor = (1, 0);
                    AdsManager.BannerAd.Position = (width, 0);
                })
                .SetLoops(-1));
            _tweens.Add(DOTween.Sequence()
                .SetDelay(8.0f)
                .AppendInterval(8.0f)
                .AppendCallback(() => {
                    Debug.Log($"Move to bottom-left");
                    AdsManager.BannerAd.Anchor = (0, 0);
                    AdsManager.BannerAd.Position = (0, 0);
                })
                .SetLoops(-1));
        }

        private void OnDestroy() {
            foreach (var tween in _tweens) {
                tween.Kill();
            }
            _tweens.Clear();
        }
    }
}