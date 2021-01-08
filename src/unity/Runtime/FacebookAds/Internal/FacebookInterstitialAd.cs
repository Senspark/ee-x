using System.Threading.Tasks;

using UnityEngine.Assertions;

namespace EE.Internal {
    internal class FacebookInterstitialAd : ObserverManager<AdObserver>, IFullScreenAd {
        private readonly IMessageBridge _bridge;
        private readonly IAsyncHelper<FullScreenAdResult> _displayer;
        private readonly FacebookAds _plugin;
        private readonly string _adId;
        private readonly MessageHelper _messageHelper;
        private bool _loadingCapped;
        private readonly IAsyncHelper<bool> _loader;

        public FacebookInterstitialAd(
            IMessageBridge bridge, IAsyncHelper<FullScreenAdResult> displayer, FacebookAds plugin, string adId) {
            _bridge = bridge;
            _displayer = displayer;
            _plugin = plugin;
            _adId = adId;
            _messageHelper = new MessageHelper("FacebookInterstitialAd", adId);
            _loadingCapped = false;
            _loader = new AsyncHelper<bool>();

            _bridge.RegisterHandler(_ => OnLoaded(), _messageHelper.OnLoaded);
            _bridge.RegisterHandler(OnFailedToLoad, _messageHelper.OnFailedToLoad);
            _bridge.RegisterHandler(OnFailedToShow, _messageHelper.OnFailedToShow);
            _bridge.RegisterHandler(_ => OnClicked(), _messageHelper.OnClicked);
            _bridge.RegisterHandler(_ => OnClosed(), _messageHelper.OnClosed);
        }

        public void Destroy() {
            _bridge.DeregisterHandler(_messageHelper.OnLoaded);
            _bridge.DeregisterHandler(_messageHelper.OnFailedToLoad);
            _bridge.DeregisterHandler(_messageHelper.OnFailedToShow);
            _bridge.DeregisterHandler(_messageHelper.OnClicked);
            _bridge.DeregisterHandler(_messageHelper.OnClosed);
            _plugin.DestroyInterstitialAd(_adId);
        }

        public bool IsLoaded {
            get {
                var response = _bridge.Call(_messageHelper.IsLoaded);
                return Utils.ToBool(response);
            }
        }

        public async Task<bool> Load() {
            if (_loadingCapped) {
                return false;
            }
            _loadingCapped = true;
            Utils.NoAwait(async () => {
                await Task.Delay(30000);
                _loadingCapped = false;
            });
            return await _loader.Process(
                () => _bridge.Call(_messageHelper.Load),
                result => {
                    // OK.
                });
        }

        public Task<FullScreenAdResult> Show() {
            return _displayer.Process(
                () => _bridge.Call(_messageHelper.Show),
                result => {
                    // OK.
                });
        }

        private void OnLoaded() {
            if (_loader.IsProcessing) {
                _loader.Resolve(true);
            } else {
                Assert.IsTrue(false);
            }
            DispatchEvent(observer => observer.OnLoaded?.Invoke());
        }

        private void OnFailedToLoad(string message) {
            if (_loader.IsProcessing) {
                _loader.Resolve(false);
            } else {
                Assert.IsTrue(false);
            }
        }

        private void OnFailedToShow(string message) {
            if (_displayer.IsProcessing) {
                _displayer.Resolve(FullScreenAdResult.Failed);
            } else {
                Assert.IsTrue(false);
            }
        }

        private void OnClicked() {
            DispatchEvent(observer => observer.OnClicked?.Invoke());
        }

        private void OnClosed() {
            if (_displayer.IsProcessing) {
                _displayer.Resolve(FullScreenAdResult.Completed);
            } else {
                Assert.IsTrue(false);
            }
        }
    }
}