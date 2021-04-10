using System;
using System.Threading.Tasks;

using Jsonite;

namespace EE.Internal {
    using Destroyer = Action;

    internal class DefaultBannerAd : ObserverManager<AdObserver>, IBannerAd {
        private const string kTag = nameof(DefaultBannerAd);
        private readonly string _prefix;
        private readonly IMessageBridge _bridge;
        private readonly Destroyer _destroyer;
        private readonly ILogger _logger;
        private readonly string _network;
        private readonly string _adId;
        private readonly MessageHelper _messageHelper;
        private readonly BannerAdHelper _helper;
        private readonly ICapper _loadCapper;
        private readonly IAsyncHelper<bool> _loader;

        public DefaultBannerAd(
            string prefix,
            IMessageBridge bridge,
            ILogger logger,
            Destroyer destroyer,
            string network,
            string adId,
            (int, int) size) {
            _prefix = prefix;
            _bridge = bridge;
            _logger = logger;
            _destroyer = destroyer;
            _network = network;
            _adId = adId;
            _messageHelper = new MessageHelper(prefix, adId);
            _helper = new BannerAdHelper(_bridge, _messageHelper, size);
            _loadCapper = new Capper(5);
            _loader = new AsyncHelper<bool>();

            _logger.Debug($"{kTag}: constructor: prefix = {_prefix} id = {_adId}");
            _bridge.RegisterHandler(_ => OnLoaded(), _messageHelper.OnLoaded);
            _bridge.RegisterHandler(message => {
                var json = (JsonObject) Json.Deserialize(message);
                OnFailedToLoad((int) json["code"], (string) json["message"]);
            }, _messageHelper.OnFailedToLoad);
            _bridge.RegisterHandler(_ => OnClicked(), _messageHelper.OnClicked);
        }

        public void Destroy() {
            _logger.Debug($"{kTag}: {nameof(Destroy)}: prefix = {_prefix} id = {_adId}");
            _bridge.DeregisterHandler(_messageHelper.OnLoaded);
            _bridge.DeregisterHandler(_messageHelper.OnFailedToLoad);
            _bridge.DeregisterHandler(_messageHelper.OnClicked);
            _destroyer();
        }

        public bool IsLoaded => _helper.IsLoaded;

        public async Task<bool> Load() {
            _logger.Debug($"{kTag}: {nameof(Load)}: prefix = {_prefix} id = {_adId} loading = {_loader.IsProcessing}");
            if (_loadCapper.IsCapped) {
                return false;
            }
            _loadCapper.Cap();
            return await _loader.Process(
                () => _helper.Load(),
                result => {
                    // OK.
                });
        }

        public bool IsVisible {
            get => _helper.IsVisible;
            set => _helper.IsVisible = value;
        }

        public (float, float) Anchor {
            get => _helper.Anchor;
            set => _helper.Anchor = value;
        }

        public (float, float) Position {
            get => _helper.Position;
            set => _helper.Position = value;
        }

        public (float, float) Size {
            get => _helper.Size;
            set => _helper.Size = value;
        }

        private void OnLoaded() {
            _logger.Debug(
                $"{kTag}: {nameof(OnLoaded)}: prefix = {_prefix} id = {_adId} loading = {_loader.IsProcessing}");
            if (_loader.IsProcessing) {
                _loader.Resolve(true);
                DispatchEvent(observer => observer.OnLoadResult?.Invoke(new AdLoadResult {
                    Network = _network,
                    Result = true
                }));
            } else {
                // Ignored.
            }
            DispatchEvent(observer => observer.OnLoaded?.Invoke());
        }

        private void OnFailedToLoad(int code, string message) {
            _logger.Debug(
                $"{kTag}: {nameof(OnFailedToLoad)}: prefix = {_prefix} id = {_adId} loading = {_loader.IsProcessing} message = {message}");
            if (_loader.IsProcessing) {
                _loader.Resolve(false);
                DispatchEvent(observer => observer.OnLoadResult?.Invoke(new AdLoadResult {
                    Network = _network,
                    Result = true,
                    ErrorCode = code,
                    ErrorMessage = message
                }));
            } else {
                // Ignored.
            }
        }

        private void OnClicked() {
            _logger.Debug($"{kTag}: {nameof(OnClicked)}: prefix = {_prefix} id = {_adId}");
            DispatchEvent(observer => observer.OnClicked?.Invoke());
        }
    }
}