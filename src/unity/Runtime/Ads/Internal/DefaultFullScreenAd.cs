using System;
using System.Threading.Tasks;

using UnityEngine.Assertions;

namespace EE.Internal {
    using Destroyer = Action;
    using ResultParser = Func<string, FullScreenAdResult>;

    internal class DefaultFullScreenAd : ObserverManager<AdObserver>, IFullScreenAd {
        private readonly string _prefix;
        private readonly IMessageBridge _bridge;
        private readonly IAsyncHelper<FullScreenAdResult> _displayer;
        private readonly Destroyer _destroyer;
        private readonly ResultParser _resultParser;
        private readonly string _adId;
        private readonly MessageHelper _messageHelper;
        private bool _loadingCapped;
        private readonly IAsyncHelper<bool> _loader;

        public AdMobInterstitialAd(
            string prefix,
            IMessageBridge bridge,
            IAsyncHelper<FullScreenAdResult> displayer,
            Destroyer destroyer,
            ResultParser resultParser,
            string adId) {
            _prefix = prefix;
            _bridge = bridge;
            _displayer = displayer;
            _destroyer = destroyer;
            _resultParser = resultParser;
            _adId = adId;
            _messageHelper = new MessageHelper(_prefix, adId);
            _loadingCapped = false;
            _loader = new AsyncHelper<bool>();

            _bridge.RegisterHandler(_ => OnLoaded(), _messageHelper.OnLoaded);
            _bridge.RegisterHandler(OnFailedToLoad, _messageHelper.OnFailedToLoad);
            _bridge.RegisterHandler(OnFailedToShow, _messageHelper.OnFailedToShow);
            _bridge.RegisterHandler(_ => OnClicked(), _messageHelper.OnClicked);
            _bridge.RegisterHandler(message => {
                var result = _resultParser(message);
                OnClosed(result);
            }, _messageHelper.OnClosed);
        }

        public void Destroy() {
            _bridge.DeregisterHandler(_messageHelper.OnLoaded);
            _bridge.DeregisterHandler(_messageHelper.OnFailedToLoad);
            _bridge.DeregisterHandler(_messageHelper.OnFailedToShow);
            _bridge.DeregisterHandler(_messageHelper.OnClicked);
            _bridge.DeregisterHandler(_messageHelper.OnClosed);
            _destroyer();
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

        private void OnClosed(FullScreenAdResult result) {
            if (_displayer.IsProcessing) {
                _displayer.Resolve(result);
            } else {
                Assert.IsTrue(false);
            }
        }
    }
}