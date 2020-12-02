using System.Threading.Tasks;

using UnityEngine.Assertions;

namespace EE.Internal {
    internal class IronSourceRewardedAd
        : ObserverManager<IRewardedAdObserver>, IRewardedAd {
        private readonly IAsyncHelper<IRewardedAdResult> _displayer;
        private readonly IronSource _plugin;
        private readonly string _adId;

        public IronSourceRewardedAd(
            IAsyncHelper<IRewardedAdResult> displayer, IronSource plugin, string adId) {
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

        public Task<IRewardedAdResult> Show() {
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
                _displayer.Resolve(IRewardedAdResult.Failed);
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
                    ? IRewardedAdResult.Completed
                    : IRewardedAdResult.Canceled);
            } else {
                Assert.IsTrue(false);
            }
        }
    }
}