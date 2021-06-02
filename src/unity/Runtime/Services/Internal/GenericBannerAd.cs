using System.Threading.Tasks;

namespace EE.Internal {
    internal class GenericBannerAd : ObserverManager<AdObserver>, IBannerAd {
        private readonly IBannerAd _ad;
        private readonly ICapper _loadCapper;
        private readonly IRetrier _loadRetrier;
        private readonly ObserverHandle _handle;
        private readonly int _screenHeight;

        public GenericBannerAd(
            IBannerAd ad,
            ICapper loadCapper,
            IRetrier loadRetrier) {
            _ad = ad;
            _loadCapper = loadCapper;
            _loadRetrier = loadRetrier;
            _handle = new ObserverHandle();
            _handle.Bind(ad).AddObserver(new AdObserver {
                OnLoaded = () => DispatchEvent(observer =>
                    observer.OnLoaded?.Invoke()),

                OnLoadResult = result => DispatchEvent(observer =>
                    observer.OnLoadResult?.Invoke(result)),

                OnClicked = () => DispatchEvent(observer =>
                    observer.OnClicked?.Invoke())
            });
            (_, _screenHeight) = Platform.GetViewSize();
        }

        public void Destroy() {
            _ad.Destroy();
            _handle.Clear();
            _loadRetrier.Stop();
        }

        public bool IsLoaded => _ad.IsLoaded;

        public async Task<bool> Load() {
            var result = await LoadInternal();
            if (result) {
                _loadRetrier.Stop();
            } else {
                Utils.NoAwait(async () => { //
                    await _loadRetrier.Process(async () => await LoadInternal());
                });
            }
            return result;
        }

        private async Task<bool> LoadInternal() {
            if (_loadCapper.IsCapped) {
                return false;
            }
            _loadCapper.Cap();
            return await _ad.Load();
        }

        public (float, float) Anchor {
            get {
                var (x, y) = _ad.Anchor;
                return (x, 1 - y);
            }
            set {
                var (x, y) = value;
                _ad.Anchor = (x, 1 - y);
            }
        }

        public (float, float) Position {
            get {
                var (x, y) = _ad.Position;
                return (x, _screenHeight - y);
            }
            set {
                var (x, y) = value;
                _ad.Position = (x, _screenHeight - y);
            }
        }

        public (float, float) Size {
            get => _ad.Size;
            set => _ad.Size = value;
        }

        public bool IsVisible {
            get => _ad.IsVisible;
            set => _ad.IsVisible = value;
        }
    }
}