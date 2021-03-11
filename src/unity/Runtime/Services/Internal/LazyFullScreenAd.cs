using System.Threading.Tasks;

namespace EE.Internal {
    public class LazyFullScreenAd : ObserverManager<AdObserver>, IFullScreenAd {
        private IFullScreenAd _ad;
        private readonly ICapper _displayCapper;
        private readonly ObserverHandle _handle;

        public LazyFullScreenAd(ICapper displayCapper) {
            _displayCapper = displayCapper;
            _handle = new ObserverHandle();
        }

        public IFullScreenAd Ad {
            get => _ad;
            set {
                _handle.Clear();
                if (value == null) {
                    return;
                }
                _handle.Bind(value)
                    .AddObserver(new AdObserver {
                        OnLoaded = () => DispatchEvent(observer =>
                            observer.OnLoaded?.Invoke()),

                        OnLoadResult = result => DispatchEvent(observer =>
                            observer.OnLoadResult?.Invoke(result)),

                        OnClicked = () => DispatchEvent(observer =>
                            observer.OnClicked?.Invoke())
                    });
                _ad = value;
                Utils.NoAwait(async () => await _ad.Load());
            }
        }

        public void Destroy() {
            _ad?.Destroy();
            _handle.Clear();
        }

        public bool IsLoaded => _ad?.IsLoaded ?? false;

        public async Task<bool> Load() {
            if (_ad == null) {
                return false;
            }
            return await _ad.Load();
        }

        public async Task<AdResult> Show() {
            if (_ad == null) {
                return AdResult.NotInitialized;
            }
            if (_displayCapper.IsCapped) {
                return AdResult.Capped;
            }
            var result = await _ad.Show();
            _displayCapper.Cap();
            return result;
        }
    }
}