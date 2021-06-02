using System.Threading.Tasks;

namespace EE.Internal {
    internal class GuardedFullScreenAd : GuardedAd, IFullScreenAd {
        private readonly IFullScreenAd _ad;
        private bool _displaying;

        protected override bool IsDisplaying => _displaying;

        public GuardedFullScreenAd(
            IFullScreenAd ad,
            ICapper capper,
            IRetrier retrier) : base(ad, capper, retrier) {
            _ad = ad;
        }

        public async Task<AdResult> Show() {
            if (!IsLoaded) {
                return AdResult.Failed;
            }
            if (IsLoading) {
                return AdResult.Failed;
            }
            if (IsDisplaying) {
                return AdResult.Failed;
            }
            _displaying = true;
            IsLoaded = false;
            var result = await _ad.Show();
            _displaying = false;
            return result;
        }
    }
}