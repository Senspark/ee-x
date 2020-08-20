using System.Threading.Tasks;

using UnityEngine.Assertions;

namespace EE.Internal {
    internal class IronSourceInterstitialAd
        : ObserverManager<IInterstitialAdObserver>, IInterstitialAd {
        private readonly IAsyncHelper<bool> _displayer;
        private readonly IronSource _plugin;
        private readonly string _adId;
        private readonly IAsyncHelper<bool> _loader;

        public IronSourceInterstitialAd(
            IAsyncHelper<bool> displayer, IronSource plugin, string adId) {
            _displayer = displayer;
            _plugin = plugin;
            _adId = adId;
            _loader = new AsyncHelper<bool>();
        }

        public void Destroy() {
            _plugin.DestroyInterstitialAd(_adId);
        }

        public bool IsLoaded => _plugin.HasInterstitialAd();

        public Task<bool> Load() {
            return _loader.Process(
                () => _plugin.LoadInterstitialAd(),
                result => {
                    // OK.
                });
        }

        public Task<bool> Show() {
            return _displayer.Process(
                () => _plugin.ShowInterstitialAd(_adId),
                result => {
                    // OK.
                });
        }

        internal void OnLoaded() {
            if (_loader.IsProcessing) {
                _loader.Resolve(true);
            } else {
                Assert.IsTrue(false);
            }
            DispatchEvent(observer => observer.OnLoaded?.Invoke());
        }

        internal void OnFailedToLoad(string message) {
            if (_loader.IsProcessing) {
                _loader.Resolve(false);
            } else {
                Assert.IsTrue(false);
            }
        }

        internal void OnFailedToShow(string message) {
            if (_displayer.IsProcessing) {
                _displayer.Resolve(false);
            } else {
                Assert.IsTrue(false);
            }
        }

        internal void OnClicked() {
            DispatchEvent(observer => observer.OnClicked?.Invoke());
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