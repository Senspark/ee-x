using System.Collections.Generic;

using DG.Tweening;

using UnityEngine;

namespace EETest {
    public class BannerAdScene : MonoBehaviour {
        private readonly List<Tween> _tweens = new List<Tween>();
        private int _viewWidth;
        private int _viewHeight;

        public EE.IAdsManager AdsManager { get; set; }

        private void Awake() {
            (_viewWidth, _viewHeight) = EE.Platform.GetViewSize();
        }

        public void Execute() {
            var density = EE.Platform.GetDensity();
            var bannerAd = AdsManager.BannerAd;
            var rectangleAd = AdsManager.RectangleAd;
            bannerAd.IsVisible = true;
            bannerAd.Size = (320 * density, 50 * density);
            rectangleAd.IsVisible = true;
            rectangleAd.Size = (300 * density, 250 * density);
            _tweens.Add(DOTween.Sequence()
                .SetDelay(2.0f)
                .AppendInterval(8.0f)
                .AppendCallback(() => {
                    Debug.Log($"Move to top-left");
                    MoveToTopLeft(bannerAd);
                    MoveToBottomLeft(rectangleAd);
                })
                .SetLoops(-1));
            _tweens.Add(DOTween.Sequence()
                .SetDelay(4.0f)
                .AppendInterval(8.0f)
                .AppendCallback(() => {
                    Debug.Log($"Move to top-right");
                    MoveToTopRight(bannerAd);
                    MoveToTopLeft(rectangleAd);
                })
                .SetLoops(-1));
            _tweens.Add(DOTween.Sequence()
                .SetDelay(6.0f)
                .AppendInterval(8.0f)
                .AppendCallback(() => {
                    Debug.Log($"Move to bottom-right");
                    MoveToBottomRight(bannerAd);
                    MoveToTopRight(rectangleAd);
                })
                .SetLoops(-1));
            _tweens.Add(DOTween.Sequence()
                .SetDelay(8.0f)
                .AppendInterval(8.0f)
                .AppendCallback(() => {
                    Debug.Log($"Move to bottom-left");
                    MoveToBottomLeft(bannerAd);
                    MoveToBottomRight(rectangleAd);
                })
                .SetLoops(-1));
        }

        private void OnDestroy() {
            foreach (var tween in _tweens) {
                tween.Kill();
            }
            _tweens.Clear();
        }

        private void MoveToTopLeft(EE.IBannerAd ad) {
            ad.Anchor = (0, 1);
            ad.Position = (0, _viewHeight);
        }

        private void MoveToTopRight(EE.IBannerAd ad) {
            ad.Anchor = (1, 1);
            ad.Position = (_viewWidth, _viewHeight);
        }

        private void MoveToBottomRight(EE.IBannerAd ad) {
            ad.Anchor = (1, 0);
            ad.Position = (_viewWidth, 0);
        }

        private void MoveToBottomLeft(EE.IBannerAd ad) {
            ad.Anchor = (0, 0);
            ad.Position = (0, 0);
        }
    }
}