using System.Drawing;
using System.Threading.Tasks;

namespace EE.Internal {
    internal class AdMobBannerAd : ObserverManager<AdObserver>, IAdView {
        private readonly IMessageBridge _bridge;
        private readonly AdMob _plugin;
        private readonly string _adId;
        private readonly MessageHelper _messageHelper;
        private readonly AdViewHelper _helper;
        private readonly IAsyncHelper<bool> _loader;

        public AdMobBannerAd(
            IMessageBridge bridge, AdMob plugin, string adId, (int, int) size) {
            _bridge = bridge;
            _plugin = plugin;
            _adId = adId;
            _messageHelper = new MessageHelper("AdMobBannerAd", adId);
            _helper = new AdViewHelper(_bridge, _messageHelper, size);
            _loader = new AsyncHelper<bool>();

            _bridge.RegisterHandler(_ => {
                Thread.RunOnLibraryThread(OnLoaded);
                return "";
            }, _messageHelper.OnLoaded);
            _bridge.RegisterHandler(message => {
                Thread.RunOnLibraryThread(() => OnFailedToLoad(message));
                return "";
            }, _messageHelper.OnFailedToLoad);
            _bridge.RegisterHandler(_ => {
                Thread.RunOnLibraryThread(OnClicked);
                return "";
            }, _messageHelper.OnClicked);
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