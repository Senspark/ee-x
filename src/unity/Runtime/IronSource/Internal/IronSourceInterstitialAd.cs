using System.Threading.Tasks;

using UnityEngine.Assertions;

namespace EE.Internal {
    internal class IronSourceInterstitialAd : ObserverManager<AdObserver>, IFullScreenAd {
        private readonly IAsyncHelper<AdResult> _displayer;
        private readonly IronSource _plugin;
        private readonly string _network;
        private readonly string _adId;
        private readonly IAsyncHelper<bool> _loader;

        public IronSourceInterstitialAd(
            IAsyncHelper<AdResult> displayer,
            IronSource plugin,
            string network,
            string adId) {
            _displayer = displayer;
            _plugin = plugin;
            _network = network;
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

        public Task<AdResult> Show() {
            return _displayer.Process(
                () => _plugin.ShowInterstitialAd(_adId),
                result => {
                    // OK.
                });
        }

        internal void OnLoaded() {
            if (_loader.IsProcessing) {
                _loader.Resolve(true);
                DispatchEvent(observer => observer.OnLoadResult?.Invoke(new AdLoadResult {
                    Network = _network,
                    Result = true
                }));
            } else {
                Assert.IsTrue(false);
            }
            DispatchEvent(observer => observer.OnLoaded?.Invoke());
        }

        internal void OnFailedToLoad(int code, string message) {
            if (_loader.IsProcessing) {
                _loader.Resolve(false);
                DispatchEvent(observer => observer.OnLoadResult?.Invoke(new AdLoadResult {
                    Network = _network,
                    Result = true,
                    ErrorCode = code,
                    ErrorMessage = message
                }));
            } else {
                Assert.IsTrue(false);
            }
        }

        internal void OnFailedToShow(int code, string message) {
            if (_displayer.IsProcessing) {
                _displayer.Resolve(AdResult.Failed);
            } else {
                Assert.IsTrue(false);
            }
        }

        internal void OnClicked() {
            DispatchEvent(observer => observer.OnClicked?.Invoke());
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