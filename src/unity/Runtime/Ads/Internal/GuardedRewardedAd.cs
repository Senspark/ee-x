using System.Threading.Tasks;

namespace EE.Internal {
    internal class GuardedRewardedAd
        : ObserverManager<IRewardedAdObserver>, IRewardedAd {
        private readonly IRewardedAd _ad;
        private bool _loading;
        private bool _displaying;
        private readonly ObserverHandle _handle;
        private readonly Retrier _retrier;

        public GuardedRewardedAd(IRewardedAd ad) {
            _ad = ad;
            _handle = new ObserverHandle();
            _handle.Bind(ad).AddObserver(new IRewardedAdObserver {
                OnLoaded = () => DispatchEvent(observer => observer.OnLoaded?.Invoke()),
                OnClicked = () => DispatchEvent(observer => observer.OnClicked?.Invoke())
            });
            _retrier = new Retrier(1, 2, 64);
        }

        public void Destroy() {
            _ad.Destroy();
            _handle.Clear();
        }

        public bool IsLoaded { get; private set; }

        public async Task<bool> Load() {
            if (IsLoaded) {
                return true;
            }
            if (_displaying) {
                return false;
            }
            if (_loading) {
                return false;
            }
            _loading = true;
            IsLoaded = await _ad.Load();
            if (IsLoaded) {
                _retrier.Stop();
            } else {
                Utils.NoAwait(async () => {
                    await _retrier.Process(async () => { //
                        return IsLoaded = await _ad.Load();
                    });
                });
            }
            _loading = false;
            return IsLoaded;
        }

        public async Task<IRewardedAdResult> Show() {
            if (!IsLoaded) {
                return IRewardedAdResult.Failed;
            }
            if (_loading) {
                return IRewardedAdResult.Failed;
            }
            if (_displaying) {
                return IRewardedAdResult.Failed;
            }
            _displaying = true;
            IsLoaded = false;
            var result = await _ad.Show();
            _displaying = false;
            return result;
        }
    }
}