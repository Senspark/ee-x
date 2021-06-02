using System.Threading.Tasks;

namespace EE.Internal {
    internal abstract class GuardedAd : ObserverManager<AdObserver>, IAd {
        private readonly IAd _ad;
        private readonly ObserverHandle _handle;
        private readonly ICapper _capper;
        private readonly IRetrier _retrier;

        protected bool IsLoading { get; private set; }
        protected abstract bool IsDisplaying { get; }

        protected GuardedAd(
            IAd ad,
            ICapper capper,
            IRetrier retrier) {
            _ad = ad;
            _handle = new ObserverHandle();
            _handle.Bind(ad).AddObserver(new AdObserver {
                OnLoaded = () => DispatchEvent(observer =>
                    observer.OnLoaded?.Invoke()),

                OnLoadResult = result => DispatchEvent(observer =>
                    observer.OnLoadResult?.Invoke(result)),

                OnClicked = () => DispatchEvent(observer =>
                    observer.OnClicked?.Invoke())
            });
            _capper = capper;
            _retrier = retrier;
        }

        public void Destroy() {
            _ad.Destroy();
            _handle.Clear();
            _retrier.Stop();
        }

        public bool IsLoaded { get; protected set; }

        public async Task<bool> Load() {
            if (IsLoaded) {
                return true;
            }
            if (IsDisplaying) {
                return false;
            }
            if (IsLoading) {
                return false;
            }
            IsLoading = true;
            IsLoaded = await LoadInternal();
            if (IsLoaded) {
                _retrier.Stop();
            } else {
                Utils.NoAwait(async () => {
                    await _retrier.Process(async () => { //
                        return IsLoaded = await LoadInternal();
                    });
                });
            }
            IsLoading = false;
            return IsLoaded;
        }

        private async Task<bool> LoadInternal() {
            if (_capper.IsCapped) {
                return false;
            }
            _capper.Cap();
            return await _ad.Load();
        }
    }
}