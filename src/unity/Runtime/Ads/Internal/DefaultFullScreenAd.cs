using System;
using System.Threading.Tasks;

using SimpleJSON;

using UnityEngine.Assertions;

namespace EE.Internal {
    using Destroyer = Action;
    using ResultParser = Func<string, AdResult>;

    internal class DefaultFullScreenAd : ObserverManager<AdObserver>, IFullScreenAd {
        private const string kTag = nameof(DefaultFullScreenAd);
        private readonly string _prefix;
        private readonly IMessageBridge _bridge;
        private readonly ILogger _logger;
        private readonly IAsyncHelper<AdResult> _displayer;
        private readonly Destroyer _destroyer;
        private readonly ResultParser _resultParser;
        private readonly string _adId;
        private readonly MessageHelper _messageHelper;
        private readonly ICapper _loadCapper;
        private readonly IAsyncHelper<bool> _loader;

        public DefaultFullScreenAd(
            string prefix,
            IMessageBridge bridge,
            ILogger logger,
            IAsyncHelper<AdResult> displayer,
            Destroyer destroyer,
            ResultParser resultParser,
            string adId) {
            _prefix = prefix;
            _bridge = bridge;
            _logger = logger;
            _displayer = displayer;
            _destroyer = destroyer;
            _resultParser = resultParser;
            _adId = adId;
            _messageHelper = new MessageHelper(_prefix, adId);
            _loadCapper = new Capper(30);
            _loader = new AsyncHelper<bool>();

            _logger.Debug($"{kTag}: constructor: prefix = {_prefix} id = {_adId}");
            _bridge.RegisterHandler(_ => OnLoaded(), _messageHelper.OnLoaded);
            _bridge.RegisterHandler(message => {
                var json = JSON.Parse(message);
                OnFailedToLoad(json["code"], json["message"]);
            }, _messageHelper.OnFailedToLoad);
            _bridge.RegisterHandler(message => {
                var json = JSON.Parse(message);
                OnFailedToShow(json["code"], json["message"]);
            }, _messageHelper.OnFailedToShow);
            _bridge.RegisterHandler(_ => OnClicked(), _messageHelper.OnClicked);
            _bridge.RegisterHandler(message => {
                var result = _resultParser(message);
                OnClosed(result);
            }, _messageHelper.OnClosed);
        }

        public void Destroy() {
            _logger.Debug($"{kTag}: {nameof(Destroy)}: prefix = {_prefix} id = {_adId}");
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
            _logger.Debug($"{kTag}: {nameof(Load)}: prefix = {_prefix} id = {_adId} loading = {_loader.IsProcessing}");
            if (_loadCapper.IsCapped) {
                return false;
            }
            _loadCapper.Cap();
            return await _loader.Process(
                () => _bridge.Call(_messageHelper.Load),
                result => {
                    // OK.
                });
        }

        public Task<AdResult> Show() {
            _logger.Debug(
                $"{kTag}: {nameof(Show)}: prefix = {_prefix} id = {_adId} displaying = {_displayer.IsProcessing}");
            return _displayer.Process(
                () => _bridge.Call(_messageHelper.Show),
                result => {
                    // OK.
                });
        }

        private void OnLoaded() {
            _logger.Debug(
                $"{kTag}: {nameof(OnLoaded)}: prefix = {_prefix} id = {_adId} loading = {_loader.IsProcessing}");
            if (_loader.IsProcessing) {
                _loader.Resolve(true);
            } else {
                Assert.IsTrue(false);
            }
            DispatchEvent(observer => observer.OnLoaded?.Invoke());
        }

        private void OnFailedToLoad(int code, string message) {
            _logger.Debug(
                $"{kTag}: {nameof(OnFailedToLoad)}: prefix = {_prefix} id = {_adId} loading = {_loader.IsProcessing} message = {message}");
            if (_loader.IsProcessing) {
                _loader.Resolve(false);
            } else {
                Assert.IsTrue(false);
            }
        }

        private void OnFailedToShow(int code, string message) {
            _logger.Debug(
                $"{kTag}: {nameof(OnFailedToLoad)}: prefix = {_prefix} id = {_adId} displaying = {_displayer.IsProcessing} message = {message}");
            if (_displayer.IsProcessing) {
                _displayer.Resolve(AdResult.Failed);
            } else {
                Assert.IsTrue(false);
            }
        }

        private void OnClicked() {
            _logger.Debug($"{kTag}: {nameof(OnClicked)}: prefix = {_prefix} id = {_adId}");
            DispatchEvent(observer => observer.OnClicked?.Invoke());
        }

        private void OnClosed(AdResult result) {
            _logger.Debug(
                $"{kTag}: {nameof(OnClosed)}: prefix = {_prefix} id = {_adId} displaying = {_displayer.IsProcessing}");
            if (_displayer.IsProcessing) {
                _displayer.Resolve(result);
            } else {
                Assert.IsTrue(false);
            }
        }
    }
}