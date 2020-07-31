using System.Threading.Tasks;

namespace EE.Internal {
    public class GuardedRewardedAd
        : ObserverManager<IRewardedAdObserver>, IRewardedAd {
        private readonly IRewardedAd _ad;
        private bool _loading;
        private bool _displaying;
        private readonly ObserverHandle _handle;

        public GuardedRewardedAd(IRewardedAd ad) {
            _ad = ad;
            _handle = new ObserverHandle();
            _handle.Bind(ad).AddObserver(new IRewardedAdObserver {
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

        public async Task<IRewardedAdResult> Show() {
            if (!IsLoaded) {
                return IRewardedAdResult.Failed;
            }
            if (_loading) {
                return IRewardedAdResult.Failed;
            }
            if (_displaying) {
                // Waiting.
                return await _ad.Show();
            }
            _displaying = true;
            var result = await _ad.Show();
            _displaying = false;
            if (result == IRewardedAdResult.Failed) {
                // Failed to show, can use this ad again.
            } else {
                IsLoaded = false;
            }
            return result;
        }
    }
}