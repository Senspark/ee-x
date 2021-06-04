namespace EE.Internal {
    internal class GenericBannerAd : GenericAd, IBannerAd {
        private readonly IBannerAd _ad;
        private readonly int _screenHeight;

        public GenericBannerAd(
            IBannerAd ad,
            ICapper loadCapper,
            IRetrier loadRetrier) : base(ad, loadCapper, loadRetrier) {
            _ad = ad;
            (_, _screenHeight) = Platform.GetViewSize();
        }

        public (float, float) Anchor {
            get {
                var (x, y) = _ad.Anchor;
                return (x, 1 - y);
            }
            set {
                var (x, y) = value;
                _ad.Anchor = (x, 1 - y);
            }
        }

        public (float, float) Position {
            get {
                var (x, y) = _ad.Position;
                return (x, _screenHeight - y);
            }
            set {
                var (x, y) = value;
                _ad.Position = (x, _screenHeight - y);
            }
        }

        public (float, float) Size {
            get => _ad.Size;
            set => _ad.Size = value;
        }

        public bool IsVisible {
            get => _ad.IsVisible;
            set => _ad.IsVisible = value;
        }
    }
}