using System.Threading.Tasks;

using UnityEngine.Assertions;

namespace EE.Internal {
    internal class IronSourceRewardedAd : ObserverManager<AdObserver>, IFullScreenAd {
        private readonly IAsyncHelper<FullScreenAdResult> _displayer;
        private readonly IronSource _plugin;
        private readonly string _adId;

        public IronSourceRewardedAd(
            IAsyncHelper<FullScreenAdResult> displayer, IronSource plugin, string adId) {
            _displayer = displayer;
            _plugin = plugin;
            _adId = adId;
        }

        public void Destroy() {
            _plugin.DestroyRewardedAd(_adId);
        }

        public bool IsLoaded => _plugin.HasRewardedAd();

        public Task<bool> Load() {
            return Task.FromResult(IsLoaded);
        }

        public Task<FullScreenAdResult> Show() {
            return _displayer.Process(
                () => _plugin.ShowRewardedAd(_adId),
                result => {
                    // OK.
                });
        }

        internal void OnLoaded() {
            DispatchEvent(observer => observer.OnLoaded?.Invoke());
        }

        internal void OnFailedToShow(string message) {
            if (_displayer.IsProcessing) {
                _displayer.Resolve(FullScreenAdResult.Failed);
            } else {
                Assert.IsTrue(false);
            }
        }

        internal void OnClicked() {
            DispatchEvent(observer => observer.OnClicked?.Invoke());
        }

        internal void OnClosed(bool rewarded) {
            if (_displayer.IsProcessing) {
                _displayer.Resolve(rewarded
                    ? FullScreenAdResult.Completed
                    : FullScreenAdResult.Canceled);
            } else {
                Assert.IsTrue(false);
            }
        }
    }
}