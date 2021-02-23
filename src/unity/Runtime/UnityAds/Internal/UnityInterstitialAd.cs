using System.Threading.Tasks;

using UnityEngine.Assertions;

namespace EE.Internal {
    internal class UnityInterstitialAd : ObserverManager<AdObserver>, IFullScreenAd {
        private readonly IAsyncHelper<AdResult> _displayer;
        private readonly UnityAds _plugin;
        private readonly string _adId;

        public UnityInterstitialAd(
            IAsyncHelper<AdResult> displayer, UnityAds plugin, string adId) {
            _displayer = displayer;
            _plugin = plugin;
            _adId = adId;
        }

        public void Destroy() {
            _plugin.DestroyFullScreenAd(_adId);
        }

        public bool IsLoaded => _plugin.HasRewardedAd(_adId);

        public Task<bool> Load() {
            Utils.NoAwait(async () => { //
                await _plugin.LoadRewardedAd(_adId);
            });
            return Task.FromResult(IsLoaded);
        }

        public Task<AdResult> Show() {
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
                _displayer.Resolve(AdResult.Failed);
            } else {
                Assert.IsTrue(false);
            }
        }

        internal void OnClosed() {
            if (_displayer.IsProcessing) {
                _displayer.Resolve(AdResult.Completed);
            } else {
                Assert.IsTrue(false);
            }
        }
    }
}