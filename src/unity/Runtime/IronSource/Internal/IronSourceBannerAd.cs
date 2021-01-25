using System.Threading.Tasks;

namespace EE.Internal {
    internal class IronSourceBannerAd : ObserverManager<AdObserver>, IBannerAd {
        private readonly IMessageBridge _bridge;
        private readonly IronSource _plugin;
        private readonly string _adId;
        private readonly MessageHelper _messageHelper;
        private readonly BannerAdHelper _helper;
        private readonly IAsyncHelper<bool> _loader;

        public IronSourceBannerAd(
            IMessageBridge bridge, IronSource plugin, string adId, (int, int) size) {
            _bridge = bridge;
            _plugin = plugin;
            _adId = adId;
            _messageHelper = new MessageHelper("IronSourceAd", adId);
            _helper = new BannerAdHelper(_bridge, _messageHelper, size);
            _loader = new AsyncHelper<bool>();

            _bridge.RegisterHandler(_ => OnLoaded(), _messageHelper.OnLoaded);
            _bridge.RegisterHandler(OnFailedToLoad, _messageHelper.OnFailedToLoad);
            _bridge.RegisterHandler(_ => OnClicked(), _messageHelper.OnClicked);
        }

        public void Destroy() {
            _bridge.DeregisterHandler(_messageHelper.OnLoaded);
            _bridge.DeregisterHandler(_messageHelper.OnFailedToLoad);
            _bridge.DeregisterHandler(_messageHelper.OnClicked);
            _plugin.DestroyBannerAd(_adId);
        }

        public bool IsLoaded => _helper.IsLoaded;

        public Task<bool> Load() {
            return _loader.Process(
                () => _helper.Load(),
                result => {
                    // OK.
                });
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

        public bool IsVisible {
            get => _helper.IsVisible;
            set => _helper.IsVisible = value;
        }

        private void OnLoaded() {
            if (_loader.IsProcessing) {
                _loader.Resolve(true);
            } else {
                // Ignored.
            }
            DispatchEvent(observer => observer.OnLoaded?.Invoke());
        }

        private void OnFailedToLoad(string message) {
            if (_loader.IsProcessing) {
                _loader.Resolve(false);
            } else {
                // Ignored.
            }
        }

        private void OnClicked() {
            DispatchEvent(observer => observer.OnClicked?.Invoke());
        }
    }
}