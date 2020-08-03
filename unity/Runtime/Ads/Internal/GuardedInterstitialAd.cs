using System.Threading.Tasks;

namespace EE.Internal {
    public class GuardedInterstitialAd
        : ObserverManager<IInterstitialAdObserver>, IInterstitialAd {
        private readonly IInterstitialAd _ad;
        private bool _loading;
        private bool _displaying;
        private readonly ObserverHandle _handle;

        public GuardedInterstitialAd(IInterstitialAd ad) {
            _ad = ad;
            _handle = new ObserverHandle();
            _handle.Bind(ad).AddObserver(new IInterstitialAdObserver {
                OnLoaded = () => {
                    IsLoaded = true;
                    DispatchEvent(observer => observer.OnLoaded?.Invoke());
                },
                OnClicked = () => DispatchEvent(observer => observer.OnClicked?.Invoke())
            });
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
                // Waiting.
                return await _ad.Load();
            }
            _loading = true;
            var result = await _ad.Load();
            _loading = false;
            return result;
        }

        public async Task<bool> Show() {
            if (!IsLoaded) {
                return false;
            }
            if (_loading) {
                return false;
            }
            if (_displaying) {
                // Waiting.
                return await _ad.Show();
            }
            _displaying = true;
            var result = await _ad.Show();
            _displaying = false;
            if (!result) {
                // Failed to show, can use this ad again.
            } else {
                IsLoaded = false;
            }
            return result;
        }
    }
}