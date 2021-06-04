using System.Threading.Tasks;

namespace EE.Internal {
    internal class GenericAd : ObserverManager<AdObserver>, IAd {
        private readonly IAd _ad;
        private readonly ICapper _loadCapper;
        private readonly IRetrier _loadRetrier;
        private readonly ObserverHandle _handle;

        protected GenericAd(
            IAd ad,
            ICapper loadCapper,
            IRetrier loadRetrier) {
            _ad = ad;
            _loadCapper = loadCapper;
            _loadRetrier = loadRetrier;
            _handle = new ObserverHandle();
            _handle.Bind(_ad)
                .AddObserver(new AdObserver {
                    OnLoaded = () => DispatchEvent(observer =>
                        observer.OnLoaded?.Invoke()),

                    OnLoadResult = result => DispatchEvent(observer =>
                        observer.OnLoadResult?.Invoke(result)),

                    OnClicked = () => DispatchEvent(observer =>
                        observer.OnClicked?.Invoke())
                });
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
    }
}