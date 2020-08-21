using System.Threading.Tasks;

using UnityEngine.Assertions;

namespace EE.Internal {
    internal class UnityInterstitialAd
        : ObserverManager<IInterstitialAdObserver>, IInterstitialAd {
        private readonly IAsyncHelper<bool> _displayer;
        private readonly UnityAds _plugin;
        private readonly string _adId;

        public UnityInterstitialAd(
            IAsyncHelper<bool> displayer, UnityAds plugin, string adId) {
            _displayer = displayer;
            _plugin = plugin;
            _adId = adId;
        }

        public void Destroy() {
            _plugin.DestroyInterstitialAd(_adId);
        }

        public bool IsLoaded => _plugin.HasRewardedAd(_adId);

        public Task<bool> Load() {
            // No effect.
            return Task.FromResult(false);
        }

        public Task<bool> Show() {
            return _displayer.Process(
                () => _plugin.ShowRewardedAd(_adId),
                result => {
                    // OK.
                });
        }

        internal void OnLoaded() {
            DispatchEvent(observer => observer?.OnLoaded());
        }

        internal void OnFailedToShow(string message) {
            if (_displayer.IsProcessing) {
                _displayer.Resolve(false);
            } else {
                Assert.IsTrue(false);
            }
        }

        internal void OnClosed() {
            if (_displayer.IsProcessing) {
                _displayer.Resolve(true);
            } else {
                Assert.IsTrue(false);
            }
        }
    }
}