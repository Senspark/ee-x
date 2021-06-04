namespace EE.Internal {
    internal class GuardedBannerAd : GuardedAd, IBannerAd {
        private readonly IBannerAd _ad;

        protected override bool IsDisplaying => false;

        public GuardedBannerAd(IBannerAd ad) : base(ad) {
            _ad = ad;
        }

        public (float, float) Anchor {
            get => _ad.Anchor;
            set => _ad.Anchor = value;
        }

        public (float, float) Position {
            get => _ad.Position;
            set => _ad.Position = value;
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