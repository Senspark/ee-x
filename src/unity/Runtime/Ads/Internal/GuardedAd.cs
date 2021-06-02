using System.Threading.Tasks;

namespace EE.Internal {
    internal abstract class GuardedAd : ObserverManager<AdObserver>, IAd {
        private readonly IAd _ad;
        private readonly ObserverHandle _handle;

        protected bool IsLoading { get; private set; }
        protected abstract bool IsDisplaying { get; }

        protected GuardedAd(IAd ad) {
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
        }

        public void Destroy() {
            _ad.Destroy();
            _handle.Clear();
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
            IsLoaded = await _ad.Load();
            IsLoading = false;
            return IsLoaded;
        }
    }
}