using System.Threading.Tasks;

using UnityEngine.Assertions;

namespace EE.Internal {
    internal class IronSourceRewardedAd : ObserverManager<AdObserver>, IFullScreenAd {
        private readonly IAsyncHelper<AdResult> _displayer;
        private readonly IronSource _plugin;
        private readonly string _adId;

        public IronSourceRewardedAd(
            IAsyncHelper<AdResult> displayer, IronSource plugin, string adId) {
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

        public Task<AdResult> Show() {
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
                _displayer.Resolve(AdResult.Failed);
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
                    ? AdResult.Completed
                    : AdResult.Canceled);
            } else {
                Assert.IsTrue(false);
            }
        }
    }
}