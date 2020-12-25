using System.Threading.Tasks;

namespace EE.Internal {
    internal class GuardedInterstitialAd
        : ObserverManager<IInterstitialAdObserver>, IInterstitialAd {
        private readonly IInterstitialAd _ad;
        private bool _loading;
        private bool _displaying;
        private readonly ObserverHandle _handle;
        private readonly Retrier _retrier;

        public GuardedInterstitialAd(IInterstitialAd ad) {
            _ad = ad;
            _handle = new ObserverHandle();
            _handle.Bind(ad).AddObserver(new IInterstitialAdObserver {
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

        public async Task<bool> Show() {
            if (!IsLoaded) {
                return false;
            }
            if (_loading) {
                return false;
            }
            if (_displaying) {
                return false;
            }
            _displaying = true;
            IsLoaded = false;
            var result = await _ad.Show();
            _displaying = false;
            return result;
        }
    }
}